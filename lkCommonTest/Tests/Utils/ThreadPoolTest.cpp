#include <gtest/gtest.h>

#include <lkCommon/Utils/ThreadPool.hpp>

using namespace lkCommon::Utils;

const uint64_t TASK_COUNT_UNTIL_VALUE = 1'000'000'000;


TEST(ThreadPool, Constructor)
{
    ThreadPool tp;
    EXPECT_EQ(lkCommon::System::Info::GetCPUCount(), tp.GetThreadCount());
}

TEST(ThreadPool, ConstructorSingleThread)
{
    ThreadPool tp(1);
    EXPECT_EQ(1, tp.GetThreadCount());
}

TEST(ThreadPool, AddTasksSimple)
{
    auto task = []() {
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
    auto task = []() {
        volatile uint64_t i = 0;
        while (i != TASK_COUNT_UNTIL_VALUE)
            i++;
    };

    ThreadPool tp(1);

    tp.AddTask(task);
    tp.AddTask(task);
    tp.AddTask(task);
}
