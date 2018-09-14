#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include <lkCommon/System/Info.hpp>
#include <lkCommon/lkCommon.hpp>


namespace lkCommon {
namespace Utils {

using TaskCallback = std::function<void()>;

struct Task
{
    TaskCallback function;

    Task();
    Task(TaskCallback&& callback);
    ~Task() = default;

    Task(const Task& other) = delete;
    Task& operator=(const Task& other) = delete;
    Task(Task&& other) = default;
    Task& operator=(Task&& other) = default;
};

struct Thread
{
    std::thread thread;
    Task assignedTask;
    uint16_t tid;
    bool taskReady;
    std::mutex stateMutex;
    std::condition_variable taskReadyCV;

    Thread();
    ~Thread();
};

class ThreadPool
{
    using ThreadContainer = std::vector<Thread>;
    using TaskContainer = std::queue<Task>;
    using LockGuard = std::lock_guard<std::mutex>;
    using UniqueLock = std::unique_lock<std::mutex>;

    std::condition_variable mDispatchThreadCV;
    bool mExitFlag;

    std::thread mDispatchThread;
    uint32_t mAvailableWorkerThreads;
    ThreadContainer mWorkerThreads;

    std::mutex mPoolStateMutex;
    std::condition_variable mStartupStateCV;
    std::condition_variable mTasksDoneCV;
    TaskContainer mTaskQueue;

    void DispatchThreadFunction();
    void WorkerThreadFunction(Thread& t);

public:
    ThreadPool(size_t threads = System::Info::GetCPUCount());
    ~ThreadPool();

    void AddTask(TaskCallback&& callback);
    void WaitForTasks();

    LKCOMMON_INLINE size_t GetThreadCount() const
    {
        return mWorkerThreads.size();
    }
};

} // namespace Utils
} // namespace lkCommon
