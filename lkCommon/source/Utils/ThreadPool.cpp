/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Basic Thread Pool implementation
 */

#include "lkCommon/Utils/ThreadPool.hpp"
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
    , payload()
    , taskReady(false)
    , stateMutex()
    , taskReadyCV()
{
}

Thread::~Thread()
{
}


ThreadPool::ThreadPool()
    : mDispatchThreadCV()
    , mExitFlag(false)
    , mDispatchThread()
    , mAvailableWorkerThreads(0)
    , mWorkerThreads(lkCommon::System::Info::GetCPUCount())
    , mPoolStateMutex()
    , mStartupStateCV()
    , mTasksDoneCV()
    , mTaskQueue()
{
    SpawnThreads();
}

ThreadPool::ThreadPool(size_t threads)
    : mDispatchThreadCV()
    , mExitFlag(false)
    , mDispatchThread()
    , mAvailableWorkerThreads(0)
    , mWorkerThreads(threads)
    , mPoolStateMutex()
    , mStartupStateCV()
    , mTasksDoneCV()
    , mTaskQueue()
{
    SpawnThreads();
}

ThreadPool::~ThreadPool()
{
    WaitForTasks();

    {
        LockGuard lock(mPoolStateMutex);
        mExitFlag = true; // enables flag, which will purge the thread pool
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

void ThreadPool::SpawnThreads()
{
    mDispatchThread = std::thread(&ThreadPool::DispatchThreadFunction, this);

    uint16_t tidCounter = 0;
    for (auto& t: mWorkerThreads)
    {
        t.payload.tid = tidCounter;
        t.thread = std::thread(&ThreadPool::WorkerThreadFunction, this, std::ref(t));
        tidCounter++;
    }

    {
        UniqueLock lock(mPoolStateMutex);
        mStartupStateCV.wait(lock, [this]() {
            return (mAvailableWorkerThreads == mWorkerThreads.size());
        });
    }
}

void ThreadPool::DispatchThreadFunction()
{
    LOGD("Dispatch thread started");

    while (true)
    {
        {
            UniqueLock dispatchThreadLock(mPoolStateMutex);
            mDispatchThreadCV.wait(dispatchThreadLock, [this]
            {
                return ((!mTaskQueue.empty()) && (mAvailableWorkerThreads > 0)) || mExitFlag;
            });
        }

        if (mExitFlag && mTaskQueue.empty() && mAvailableWorkerThreads == mWorkerThreads.size())
        {
            mTasksDoneCV.notify_all();
            break;
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
                {
                    break;
                }
            }

            // if there's no free thread, programmer screwed up =(
            LKCOMMON_ASSERT(thread != mWorkerThreads.size(), "There should exist a free thread on this stage of dispatcher loop");

            // prepare the thread
            Thread& t = mWorkerThreads[thread];

            // assign a task from queue and update das
            {
                LockGuard lock(mPoolStateMutex);

                mAvailableWorkerThreads--;
                t.assignedTask = std::move(mTaskQueue.front());
                mTaskQueue.pop();
            }

            {
                LockGuard lock(t.stateMutex);
                t.taskReady = true;
            }

            t.taskReadyCV.notify_all();
        }
    }

    LOGD("Dispatch thread stopped");
}

void ThreadPool::WorkerThreadFunction(Thread& self)
{
    LOGD(self.payload.tid << ": Worker thread started");

    {
        LockGuard lock(mPoolStateMutex);
        mAvailableWorkerThreads++;
        mStartupStateCV.notify_all();
    }

    while (true)
    {
        {
            UniqueLock cvLock(self.stateMutex);
            self.taskReadyCV.wait(cvLock, [this, &self]() {
                return (self.taskReady == true) || (mExitFlag == true);
            });
        }

        if (mExitFlag)
        {
            break;
        }

        // do the thing we are meant to do
        self.assignedTask.function(self.payload);

        // mark the thread as free to do other tasks
        {
            LockGuard lock(self.stateMutex);
            self.taskReady = false;
        }

        {
            LockGuard lock(mPoolStateMutex);
            mAvailableWorkerThreads++;

            if (mAvailableWorkerThreads == mWorkerThreads.size())
                mTasksDoneCV.notify_all();
        }

        // notify dispatcher that we are done
        mDispatchThreadCV.notify_all();
    }

    LOGD(self.payload.tid << ": Worker thread stopped");
}

void ThreadPool::SetUserPayloadForThread(uint16_t pid, void* payloadPtr)
{
    LKCOMMON_ASSERT(pid < mWorkerThreads.size(), "Provided PID is too high - thread does not exist");

    mWorkerThreads[pid].payload.userData = payloadPtr;
}

void ThreadPool::AddTask(TaskCallback&& callback)
{
    {
        LockGuard lock(mPoolStateMutex);
        mTaskQueue.emplace(std::move(callback));
    }
    mDispatchThreadCV.notify_all();
}

void ThreadPool::WaitForTasks()
{
    UniqueLock lock(mPoolStateMutex);
    mTasksDoneCV.wait(lock, [this]
    {
        return (mTaskQueue.empty()) && (mAvailableWorkerThreads == mWorkerThreads.size());
    });
}

} // namespace Utils
} // namespace lkCommon
