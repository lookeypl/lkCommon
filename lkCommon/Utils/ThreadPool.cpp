#include "ThreadPool.hpp"
#include <limits>


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
    , assignedTask()
    , tid(std::numeric_limits<uint16_t>::max())
    , taskReady(false)
    , stateMutex()
    , taskReadyCV()
{
}

Thread::~Thread()
{
}


ThreadPool::ThreadPool(size_t threads)
    : mDispatchThreadMutex()
    , mDispatchThreadCV()
    , mDispatchThreadExitFlag(false)
    , mAvailableWorkerThreads(0)
    , mDispatchThread()
    , mWorkerThreadStateMutex()
    , mWorkerThreads(threads)
    , mTaskQueueMutex()
    , mTaskQueue()
{
    mDispatchThread = std::thread(&ThreadPool::DispatchThreadFunction, this);

    uint16_t tidCounter = 0;
    for (auto& t: mWorkerThreads)
    {
        t.tid = tidCounter;
        t.thread = std::thread(&ThreadPool::WorkerThreadFunction, this, std::ref(t));
        tidCounter++;
    }

    {
        UniqueLock lock(mWorkerThreadStateMutex);
        mStartupStateCV.wait(lock, [this, &threads]() {
            return (mAvailableWorkerThreads == threads);
        });
    }
}

ThreadPool::~ThreadPool()
{
    WaitForTasks();

    {
        LockGuard lock(mDispatchThreadMutex);
        mDispatchThreadExitFlag = true; // enables flag, which will purge the thread pool
    }
    mDispatchThreadCV.notify_all();

    // first, leave dispatch thread
    mDispatchThread.join();

    // now all tasks should be dispatched, wait for each of worker threads
    for (auto& t: mWorkerThreads)
    {
        if (t.thread.joinable())
        {
            t.taskReadyCV.notify_all();
            t.thread.join();
        }
    }
}

void ThreadPool::DispatchThreadFunction()
{
    LOGD("Dispatch thread started");

    while (true)
    {
        {
            UniqueLock dispatchThreadLock(mDispatchThreadMutex);
            mDispatchThreadCV.wait(dispatchThreadLock, [this]
            {
                return ((!mTaskQueue.empty()) && (mAvailableWorkerThreads > 0)) || mDispatchThreadExitFlag;
            });
        }

        if (mDispatchThreadExitFlag && mTaskQueue.empty() &&
            mAvailableWorkerThreads == mWorkerThreads.size())
        {
            mTasksDoneCV.notify_all();
            return;
        }

        // TODO THIS MIGHT SPIN-LOOP DISPATCH THREAD
        // Situation where ExitFlag == true and there are no working threads available,
        // then CV above will never wait. Try to avoid that.
        if (mAvailableWorkerThreads > 0 && !mTaskQueue.empty())
        {
            // get first free thread (should be available)
            uint32_t thread = 0;
            for (thread; thread < mWorkerThreads.size(); ++thread)
            {
                if (mWorkerThreads[thread].taskReady == false)
                    break;
            }

            // if there's no free thread, programmer screwed up =(
            LKCOMMON_ASSERT(thread != mWorkerThreads.size(), "There should exist a free thread on this stage of dispatcher loop");

            // prepare the thread
            Thread& t = mWorkerThreads[thread];

            // assign a task from queue
            {
                LockGuard lock(mTaskQueueMutex);
                t.assignedTask = std::move(mTaskQueue.front());
                mTaskQueue.pop();
            }

            // update states and get ready
            {
                LockGuard lock(mWorkerThreadStateMutex);
                mWorkerThreads[thread].taskReady = true;
                mAvailableWorkerThreads--;
            }

            t.taskReadyCV.notify_all();
        }
    }

    LOGD("Dispatch thread stopped");
}

void ThreadPool::WorkerThreadFunction(Thread& self)
{
    LOGD(self.tid << ": Worker thread started");

    {
        LockGuard lock(mWorkerThreadStateMutex);
        mAvailableWorkerThreads++;
        mStartupStateCV.notify_all();
    }

    while (true)
    {
        {
            UniqueLock cvLock(self.stateMutex);
            LOGD(self.tid << ": taskReady = " << self.taskReady
                          << ", dispatchExitFlag = " << mDispatchThreadExitFlag);
            self.taskReadyCV.wait(cvLock, [this, &self]() {
                return (self.taskReady == true) || (mDispatchThreadExitFlag == true);
            });
        }

        if (mDispatchThreadExitFlag)
        {
            LOGD(self.tid << ": Exiting");
            break;
        }

        // do the thing we are meant to do
        self.assignedTask.function();

        // mark the thread as free to do other tasks
        {
            LockGuard lock(self.stateMutex);
            self.taskReady = false;
        }

        {
            LockGuard lock(mWorkerThreadStateMutex);
            mAvailableWorkerThreads++;

            if (mAvailableWorkerThreads == mWorkerThreads.size())
                mTasksDoneCV.notify_all();
        }

        // notify dispatcher that we are done
        mDispatchThreadCV.notify_all();
    }

    LOGD(self.tid << ": Worker thread stopped");
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
    LOGD("WaitForTasks invoked");

    UniqueLock lock(mTaskQueueMutex);
    mTasksDoneCV.wait(lock, [this]
    {
        bool test = (mTaskQueue.empty()) && (mAvailableWorkerThreads == mWorkerThreads.size());
        LOGD("Checking if tasks done: " << test);
        return test;
    });
}

} // namespace Utils
} // namespace lkCommon
