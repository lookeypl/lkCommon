#include "ThreadPool.hpp"


namespace lkCommon {
namespace Utils {


Task::Task()
    : function()
{
}

Task::Task(TaskCallback&& callback)
    : function(std::move(callback))
{
}


Thread::Thread()
    : thread()
    , busy(false)
{
}

Thread::~Thread()
{
}


ThreadPool::ThreadPool(size_t threads)
    : mDispatchThreadMutex()
    , mDispatchThreadCV()
    , mDispatchThreadExitFlag(false)
    , mAvailableWorkerThreads(threads)
    , mDispatchThread()
    , mWorkerThreadStateMutex()
    , mWorkerThreads(threads)
    , mTaskQueueMutex()
    , mTaskQueue()
{
    mDispatchThread = std::thread(&ThreadPool::DispatchThreadFunction, this);
}

ThreadPool::~ThreadPool()
{
    {
        LockGuard lock(mDispatchThreadMutex);
        mDispatchThreadExitFlag = true; // enables flag, which will purge the thread pool
    }
    mDispatchThreadCV.notify_all();

    // first, leave dispatch thread
    mDispatchThread.join();

    // now all tasks should be dispatched, wait for each of worker threads
    for (auto& t: mWorkerThreads)
        if (t.thread.joinable())
            t.thread.join();
}

void ThreadPool::DispatchThreadFunction()
{
    while (true)
    {
        UniqueLock dispatchThreadLock(mDispatchThreadMutex);
        mDispatchThreadCV.wait(dispatchThreadLock, [this]
        {
            return ((!mTaskQueue.empty()) && (mAvailableWorkerThreads > 0)) || mDispatchThreadExitFlag;
        });

        if (mDispatchThreadExitFlag && mTaskQueue.empty())
            return;

        // TODO THIS MIGHT SPIN-LOOP DISPATCH THREAD
        // Situation where ExitFlag == true and there are no working threads available,
        // then CV above will never wait. Try to avoid that.
        if (mAvailableWorkerThreads > 0)
        {
            LockGuard lock(mWorkerThreadStateMutex);

            // get first free thread (should be available)
            uint32_t thread = 0;
            for (thread; thread < mWorkerThreads.size(); ++thread)
            {
                if (!mWorkerThreads[thread].busy)
                    break;
            }

            // if there's no free thread, programmer screwed up =(
            LKCOMMON_ASSERT(thread != mWorkerThreads.size(), "There should exist a free thread on this stage of dispatcher loop");

            // prepare the thread
            Thread& t = mWorkerThreads[thread];
            t.busy = true;
            mAvailableWorkerThreads--;

            // join the thread to ensure it stopped execution (shouldn't take long, the thread is already done)
            if (t.thread.joinable())
                t.thread.join();

            // assign a task from queue
            {
                LockGuard taskQueueLock(mTaskQueueMutex);
                t.assignedTask = std::move(mTaskQueue.front());
                mTaskQueue.pop();
            }

            // do the job
            t.thread = std::thread(&ThreadPool::WorkerThreadFunction, this, std::ref(t));
        }
    }
}

void ThreadPool::WorkerThreadFunction(Thread& self)
{
    // do the thing we are meant to do
    self.assignedTask.function();

    // mark the thread as free to do other tasks
    {
        LockGuard lock(mWorkerThreadStateMutex);
        self.busy = false;
        mAvailableWorkerThreads++;

        if (mAvailableWorkerThreads == mWorkerThreads.size())
            mTasksDoneCV.notify_all();
    }

    // notify dispatcher that we are done
    mDispatchThreadCV.notify_all();
}

void ThreadPool::AddTask(TaskCallback&& callback)
{
    {
        LockGuard lock(mTaskQueueMutex);
        mTaskQueue.emplace(std::move(callback));
    }
    mDispatchThreadCV.notify_all();
}

void ThreadPool::WaitForTasks()
{
    UniqueLock lock(mTaskQueueMutex);
    mTasksDoneCV.wait(lock, [this]
    {
        return (mTaskQueue.empty()) && (mAvailableWorkerThreads == mWorkerThreads.size());
    });
}

} // namespace Utils
} // namespace lkCommon
