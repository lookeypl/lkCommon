#include <gtest/gtest.h>
#include <lkCommon/Utils/StaticQueue.hpp>

#include <array>

using namespace lkCommon::Utils;

const std::array<int, 6> TEST_VALUES = { 8, 2, 3, 5, 1, 9};


TEST(StaticQueue, Create)
{
    StaticQueue<int, 20> q;
    EXPECT_EQ(20, q.Capacity());
    EXPECT_EQ(0, q.Size());
}

TEST(StaticQueue, PushPop)
{
    StaticQueue<int, 20> q;
    ASSERT_EQ(20, q.Capacity());
    ASSERT_EQ(0, q.Size());

    for (const auto& v: TEST_VALUES)
    {
        q.Push(v);
    }

    for (const auto& v: TEST_VALUES)
    {
        int qv = q.Pop();
        EXPECT_EQ(v, qv);
    }
}

TEST(StaticQueue, EmplacePop)
{
    StaticQueue<int, 20> q;
    ASSERT_EQ(20, q.Capacity());
    ASSERT_EQ(0, q.Size());

    for (const auto& v: TEST_VALUES)
    {
        q.Emplace(v);
    }

    for (const auto& v: TEST_VALUES)
    {
        int qv = q.Pop();
        EXPECT_EQ(v, qv);
    }
}

TEST(StaticQueue, PushOverflow)
{
    StaticQueue<int, TEST_VALUES.size()> q;
    ASSERT_EQ(TEST_VALUES.size(), q.Capacity());
    ASSERT_EQ(0, q.Size());

    // move queue close to overflowing
    const size_t fillCount = q.Capacity() - 2;
    for (size_t i = 0; i < fillCount; ++i)
    {
        q.Push(static_cast<int>(i));
    }

    // clear the queue
    for (size_t i = 0; i < fillCount; ++i)
    {
        q.Pop();
    }

    // do the test
    for (const auto& v: TEST_VALUES)
    {
        q.Push(v);
    }

    for (const auto& v: TEST_VALUES)
    {
        int qv = q.Pop();
        EXPECT_EQ(v, qv);
    }
}

TEST(StaticQueue, EmplaceOverflow)
{
    StaticQueue<int, TEST_VALUES.size()> q;
    ASSERT_EQ(TEST_VALUES.size(), q.Capacity());
    ASSERT_EQ(0, q.Size());

    // move queue close to overflowing
    const size_t fillCount = q.Capacity() - 2;
    for (size_t i = 0; i < fillCount; ++i)
    {
        q.Emplace(i);
    }

    // clear the queue
    for (size_t i = 0; i < fillCount; ++i)
    {
        q.Pop();
    }

    // do the test
    for (const auto& v: TEST_VALUES)
    {
        q.Emplace(v);
    }

    for (const auto& v: TEST_VALUES)
    {
        int qv = q.Pop();
        EXPECT_EQ(v, qv);
    }
}

TEST(StaticQueue, SizeWithoutWrap)
{
    StaticQueue<int, 20> q;
    ASSERT_EQ(20, q.Capacity());
    ASSERT_EQ(0, q.Size());

    for (const auto& v: TEST_VALUES)
    {
        q.Emplace(v);
    }

    EXPECT_EQ(TEST_VALUES.size(), q.Size());
}

TEST(StaticQueue, SizeWithoutWrapFull)
{
    StaticQueue<int, TEST_VALUES.size()> q;
    ASSERT_EQ(TEST_VALUES.size(), q.Capacity());
    ASSERT_EQ(0, q.Size());

    for (const auto& v: TEST_VALUES)
    {
        q.Emplace(v);
    }

    EXPECT_EQ(TEST_VALUES.size(), q.Size());
}

TEST(StaticQueue, SizeAfterWrap)
{
    StaticQueue<int, 20> q;
    ASSERT_EQ(20, q.Capacity());
    ASSERT_EQ(0, q.Size());

    // move queue close to overflowing
    const size_t fillCount = q.Capacity() - 2;
    for (size_t i = 0; i < fillCount; ++i)
    {
        q.Emplace(i);
    }

    // clear the queue
    for (size_t i = 0; i < fillCount; ++i)
    {
        q.Pop();
    }

    // emplace values so queue overflows
    for (const auto& v: TEST_VALUES)
    {
        q.Emplace(v);
    }

    EXPECT_EQ(TEST_VALUES.size(), q.Size());
}

TEST(StaticQueue, SizeAfterWrapFull)
{
    StaticQueue<int, TEST_VALUES.size()> q;
    ASSERT_EQ(TEST_VALUES.size(), q.Capacity());
    ASSERT_EQ(0, q.Size());

    // move queue close to overflowing
    const size_t fillCount = q.Capacity() - 2;
    for (size_t i = 0; i < fillCount; ++i)
    {
        q.Emplace(i);
    }

    // clear the queue
    for (size_t i = 0; i < fillCount; ++i)
    {
        q.Pop();
    }

    // emplace values so queue overflows
    for (const auto& v: TEST_VALUES)
    {
        q.Emplace(v);
    }

    EXPECT_EQ(TEST_VALUES.size(), q.Size());
}

TEST(StaticQueue, EnqueueJustEnough_StartToEnd)
{
    StaticQueue<int, TEST_VALUES.size()> q;
    ASSERT_EQ(TEST_VALUES.size(), q.Capacity());
    ASSERT_EQ(0, q.Size());

    // emplace values so queue fills
    for (const auto& v: TEST_VALUES)
    {
        q.Emplace(v);
    }

    EXPECT_EQ(q.Capacity(), q.Size());
    EXPECT_EQ(TEST_VALUES.size(), q.Size());
}

TEST(StaticQueue, EnqueueJustEnough_InTheMiddle)
{
    StaticQueue<int, TEST_VALUES.size()> q;
    ASSERT_EQ(TEST_VALUES.size(), q.Capacity());
    ASSERT_EQ(0, q.Size());

    // move queue close to overflowing
    const size_t fillCount = q.Capacity() - 2;
    for (size_t i = 0; i < fillCount; ++i)
    {
        q.Emplace(i);
    }

    // clear the queue
    for (size_t i = 0; i < fillCount; ++i)
    {
        q.Pop();
    }

    // emplace values so queue fills
    for (const auto& v: TEST_VALUES)
    {
        q.Emplace(v);
    }

    EXPECT_EQ(q.Capacity(), q.Size());
    EXPECT_EQ(TEST_VALUES.size(), q.Size());
}
