#include <gtest/gtest.h>
#include <lkCommon/Utils/StaticStack.hpp>

struct StackElement
{
    int a;
    float b;

    StackElement()
        : a(0), b(0.0f)
    {
    }

    StackElement(int a, float b)
        : a(a), b(b)
    {
    }

    bool operator==(const StackElement& other) const
    {
        return (a == a) && (b == b);
    }
};

const int TEST_INT_1 = 10;
const int TEST_INT_2 = 69;
const float TEST_FLOAT_1 = 2.31f;
const float TEST_FLOAT_2 = 4.20f;

const StackElement TEST_ELEMENT_1(TEST_INT_1, TEST_FLOAT_1);
const StackElement TEST_ELEMENT_2(TEST_INT_2, TEST_FLOAT_2);

using namespace lkCommon::Utils;

TEST(StaticStack, PushPopSingle)
{
    StaticStack<StackElement, 1> stack;
    EXPECT_EQ(1, stack.Capacity());
    EXPECT_EQ(0, stack.Size());

    stack.Push(TEST_ELEMENT_1);
    EXPECT_EQ(1, stack.Size());
    EXPECT_EQ(TEST_ELEMENT_1, stack.Pop());
    EXPECT_EQ(0, stack.Size());

    stack.Push(TEST_ELEMENT_2);
    EXPECT_EQ(1, stack.Size());
    EXPECT_EQ(TEST_ELEMENT_2, stack.Pop());
    EXPECT_EQ(0, stack.Size());
}

TEST(StaticStack, EmplacePopSingle)
{
    StaticStack<StackElement, 1> stack;
    EXPECT_EQ(1, stack.Capacity());
    EXPECT_EQ(0, stack.Size());

    stack.Emplace(TEST_ELEMENT_1);
    EXPECT_EQ(1, stack.Size());
    EXPECT_EQ(TEST_ELEMENT_1, stack.Pop());
    EXPECT_EQ(0, stack.Size());

    stack.Emplace(TEST_ELEMENT_2);
    EXPECT_EQ(1, stack.Size());
    EXPECT_EQ(TEST_ELEMENT_2, stack.Pop());
    EXPECT_EQ(0, stack.Size());
}

TEST(StaticStack, PushEmplacePopMultiple)
{
    StaticStack<StackElement, 2> stack;
    EXPECT_EQ(2, stack.Capacity());
    EXPECT_EQ(0, stack.Size());

    stack.Push(TEST_ELEMENT_1);
    EXPECT_EQ(1, stack.Size());

    stack.Emplace(TEST_ELEMENT_2);
    EXPECT_EQ(2, stack.Size());

    EXPECT_EQ(TEST_ELEMENT_2, stack.Pop());
    EXPECT_EQ(1, stack.Size());

    EXPECT_EQ(TEST_ELEMENT_1, stack.Pop());
    EXPECT_EQ(0, stack.Size());
}
