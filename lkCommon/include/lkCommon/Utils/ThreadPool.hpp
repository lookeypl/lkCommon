/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Basic Thread Pool implementation
 */

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

/**
 * Collection of per-thread data.
 *
 * User can provide own data to userData pointer using
 * ThreadPool::SetUserThreadData() function.
 */
struct ThreadPayload
{
    uint16_t tid;
    void* userData;

    ThreadPayload()
        : tid(std::numeric_limits<uint16_t>::max())
        , userData(nullptr)
    {
    }
};

/**
 * Typedef for task called by ThreadPool
 */
using TaskCallback = std::function<void(ThreadPayload&)>;

/**
 * Structure representing a task to be executed on a separate thread.
 *
 * Task is non-copyable to prevent
 */
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

/**
 * Internal representation of a thread with all its required elements
 */
struct Thread
{
    std::thread thread;
    Task assignedTask;
    ThreadPayload payload;
    bool taskReady;
    std::mutex stateMutex;
    std::condition_variable taskReadyCV;

    Thread();
    ~Thread();
};

/**
 * Thread Pool class used to launch tasks on different threads.
 *
 * Main purpose of this utility is to allow performing independent tasks while
 * maximizing CPU usage. By default, the module initializes with amount of
 * threads matching logical CPU count in the system.
 *
 * The pool spawns one dispatcher thread responsible for assigning tasks and
 * N worker threads to perform Tasks provided by user. Tasks added by user are
 * enqueued and performed in a FIFO basis. There is no dependency mechanism
 * provided and tasks are not synchronized between each other. If tasks happen
 * to share a common resource, it is user's duty to ensure there is
 * no race/synchronization issues while accessing it.
 */
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

    void SpawnThreads();
    void DispatchThreadFunction();
    void WorkerThreadFunction(Thread& t);

public:
    /**
     * Default ThreadPool constructor. Spawns dispatcher thread and amount of
     * worker threads equal to logical CPU count in current system.
     *
     * Constructor is left when all worker threads report ready to process
     * tasks.
     */
    ThreadPool();

    /**
     * Initializes ThreadPool with provided thread count. Spawns dispatcher
     * thread and @p threads count of worker threads.
     *
     * Constructor is left when all worker threads report ready to process
     * tasks.
     *
     * @p[in] threads Amount of worker threads to spawn. If equal to zero,
     *                constructor will acquire logical CPU count available
     *                in the system and spawn as many threads as needed.
     */
    ThreadPool(size_t threads);

    /**
     * Destroys Thread Pool. If there are any tasks left to complete, destructor
     * waits for them to finish, signals end of work and joins all threads
     * created on constructor stage.
     */
    ~ThreadPool();

    /**
     * Sets custom data to be provided per thread.
     *
     * @p[in] tid        ID of thread to attach payload to.
     * @p[in] payloadPtr Pointer to custom user data to attach.
     *
     * After this call, each time thread @p tid starts a task, it's going to
     * have @p payloadPtr attached to its ThreadPayload structure.
     */
    void SetUserPayloadForThread(uint16_t tid, void* payloadPtr);

    /**
     * Adds new task to Task queue which will execute @p callback callback.
     *
     * @p[in] callback Callback for task to be called by Thread Pool. Callback
     *                 is moved by this function, so after completing it
     *                 the parameter is left in unspecified state.
     *
     * @note ThreadPool will provide a ThreadPayload object as a parameter for
     *       callback. Thus it is required, that user's callback contains a
     *       ThreadPayload reference argument.
     *
     * @note This function is thread-safe and can be called by multiple threads.
     */
    void AddTask(TaskCallback&& callback);

    /**
     * Waits for task queue to empty and for worker threads to finish executing
     * tasks.
     *
     * After exiting this function user can be sure there is no more tasks left
     * to perform in queue and all worker threads are in idle state.
     */
    void WaitForTasks();

    /**
     * Returns worker thread count which is currently used by Pool.
     *
     * @result Amount of threads used by Pool.
     */
    LKCOMMON_INLINE size_t GetWorkerThreadCount() const
    {
        return mWorkerThreads.size();
    }
};

} // namespace Utils
} // namespace lkCommon
