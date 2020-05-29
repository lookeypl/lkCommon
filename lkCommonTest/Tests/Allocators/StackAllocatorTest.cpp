#include "lkCommon/Allocators/StackAllocator.hpp"
#include <gtest/gtest.h>

using namespace lkCommon::Allocators;

namespace {

const size_t ALLOCATOR_SIZE = 4096;
const size_t ALLOCATION_SIZE_SMALL = 16;
const uint32_t MAGIC_VALUE = 0x042069;

} // namespace


TEST(StackAllocator, Constructor)
{
    StackAllocator<ALLOCATOR_SIZE> allocator;
    EXPECT_EQ(0, allocator.GetUsedMemory());
}

TEST(StackAllocator, Allocate)
{
    StackAllocator<ALLOCATOR_SIZE> allocator;

    ASSERT_EQ(0, allocator.GetUsedMemory());

    EXPECT_NE(nullptr, allocator.Allocate(ALLOCATION_SIZE_SMALL));
    EXPECT_EQ(ALLOCATION_SIZE_SMALL, allocator.GetUsedMemory());
}

TEST(StackAllocator, AllocateToSize)
{
    StackAllocator<ALLOCATOR_SIZE> allocator;

    ASSERT_EQ(0, allocator.GetUsedMemory());

    EXPECT_NE(nullptr, allocator.Allocate(ALLOCATOR_SIZE));
    EXPECT_EQ(ALLOCATOR_SIZE, allocator.GetUsedMemory());
}

TEST(StackAllocator, Free)
{
    StackAllocator<ALLOCATOR_SIZE> allocator;

    ASSERT_EQ(0, allocator.GetUsedMemory());

    uint32_t* ptr = reinterpret_cast<uint32_t*>(
        allocator.Allocate(ALLOCATION_SIZE_SMALL)
    );
    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(ALLOCATION_SIZE_SMALL, allocator.GetUsedMemory());

    *ptr = MAGIC_VALUE;

    // Calling Free() should do nothing
    allocator.Free(ptr);

    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(MAGIC_VALUE, *ptr);
    EXPECT_EQ(ALLOCATION_SIZE_SMALL, allocator.GetUsedMemory());
}

TEST(StackAllocator, ClearAndAllocate)
{
    StackAllocator<ALLOCATOR_SIZE> allocator;

    ASSERT_EQ(0, allocator.GetUsedMemory());

    EXPECT_NE(nullptr, allocator.Allocate(ALLOCATION_SIZE_SMALL));
    EXPECT_EQ(ALLOCATION_SIZE_SMALL, allocator.GetUsedMemory());

    allocator.Clear();
    EXPECT_EQ(0, allocator.GetUsedMemory());

    EXPECT_NE(nullptr, allocator.Allocate(ALLOCATION_SIZE_SMALL));
    EXPECT_EQ(ALLOCATION_SIZE_SMALL, allocator.GetUsedMemory());
}
