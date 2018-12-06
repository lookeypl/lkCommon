#include <gtest/gtest.h>

#include <lkCommon/Utils/ThreadPool.hpp>

using namespace lkCommon::Utils;

const uint64_t TASK_COUNT_UNTIL_VALUE = 400'000'000;
const uint64_t TASK_COUNT_UNTIL_VALUE_SMALL = 100'000;
const uint32_t PAYLOAD_CONST_VALUE_TO_ADD = 42;


TEST(ThreadPool, Constructor)
{
    ThreadPool tp;
    EXPECT_EQ(lkCommon::System::Info::GetCPUCount(), tp.GetWorkerThreadCount());
}

TEST(ThreadPool, ConstructorSingleThread)
{
    ThreadPool tp(1);
    EXPECT_EQ(1, tp.GetWorkerThreadCount());
}

TEST(ThreadPool, AddTasksSimple)
{
    auto task = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE)
            i++;
    };

    ThreadPool tp(3);

    tp.AddTask(task);
    tp.AddTask(task);
    tp.AddTask(task);
}

TEST(ThreadPool, AddTasksSimpleSingleThread)
{
    auto task = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE)
            i++;
    };

    ThreadPool tp(1);

    tp.AddTask(task);
    tp.AddTask(task);
    tp.AddTask(task);
}

TEST(ThreadPool, AddTasksComplex)
{
    auto task = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE)
            i++;
    };

    auto taskDouble = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE * 2)
            i++;
    };

    auto taskHalf = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE / 2)
            i++;
    };

    // init pool
    ThreadPool tp(3);

    // add first tasks
    tp.AddTask(task);
    tp.AddTask(taskDouble);
    tp.AddTask(taskHalf);

    // synchronization point
    tp.WaitForTasks();

    // add more tasks
    tp.AddTask(task);
    tp.AddTask(taskDouble);
    tp.AddTask(taskDouble);
    tp.AddTask(taskHalf);
    tp.AddTask(taskHalf);
    tp.AddTask(task);
}

TEST(ThreadPool, AddTasksDoubleThreadCount)
{
    auto task = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE)
            i++;
    };

    auto taskDouble = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE * 2)
            i++;
    };

    auto taskHalf = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE / 2)
            i++;
    };

    size_t cpuCount = lkCommon::System::Info::GetCPUCount() * 2;

    // init pool
    ThreadPool tp(cpuCount);

    size_t taskSelector = 0;
    const size_t taskCollectionCount = 3;

    for (size_t i = 0; i < cpuCount * 2; ++i)
    {
        taskSelector = i % taskCollectionCount;
        switch (taskSelector)
        {
        case 0: tp.AddTask(task); break;
        case 1: tp.AddTask(taskDouble); break;
        case 2: tp.AddTask(taskHalf); break;
        default: break;
        }
    }
}

TEST(ThreadPool, StressTest)
{
    auto task = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE_SMALL)
            i++;
    };

    auto taskDouble = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE_SMALL * 2)
            i++;
    };

    auto taskHalf = [](ThreadPayload&) {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE_SMALL / 2)
            i++;
    };

    const size_t taskCount = 200;

    // init pool
    ThreadPool tp;

    size_t taskSelector = 0;
    const size_t taskCollectionCount = 3;

    for (size_t j = 0; j < 30; ++j)
    {
        for (size_t i = 0; i < taskCount; ++i)
        {
            taskSelector = i % taskCollectionCount;
            switch (taskSelector)
            {
            case 0: tp.AddTask(task); break;
            case 1: tp.AddTask(taskDouble); break;
            case 2: tp.AddTask(taskHalf); break;
            default: break;
            }
        }

        tp.WaitForTasks();
    }
}

TEST(ThreadPool, PayloadTest)
{
    uint32_t payload1 = 0;
    uint32_t payload2 = 0;
    uint32_t payload3 = 0;

    auto task = [](ThreadPayload& payload)
    {
        uint32_t* data = reinterpret_cast<uint32_t*>(payload.userData);

        EXPECT_NE(nullptr, data);

        if (data)
            *data = PAYLOAD_CONST_VALUE_TO_ADD + static_cast<uint32_t>(payload.tid);
    };

    ThreadPool tp(3);

    tp.SetUserPayloadForThread(0, &payload1);
    tp.SetUserPayloadForThread(1, &payload2);
    tp.SetUserPayloadForThread(2, &payload3);

    tp.AddTask(task);
    tp.AddTask(task);
    tp.AddTask(task);

    tp.WaitForTasks();

    EXPECT_EQ(PAYLOAD_CONST_VALUE_TO_ADD + 0, payload1);
    EXPECT_EQ(PAYLOAD_CONST_VALUE_TO_ADD + 1, payload2);
    EXPECT_EQ(PAYLOAD_CONST_VALUE_TO_ADD + 2, payload3);
}
