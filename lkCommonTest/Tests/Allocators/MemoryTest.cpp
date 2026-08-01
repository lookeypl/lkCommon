#include "lkCommon/Allocators/Memory.hpp"
#include "lkCommon/Allocators/ArenaAllocator.hpp"
#include "lkCommon/Allocators/StackAllocator.hpp"
#include <gtest/gtest.h>


/**
 * Tests in this file are mostly compiler tests to ensure that Memory template
 * works and compiles properly. More detailed tests (for corner cases etc.)
 * should exist in respective Allocator's test source.
 */


using namespace lkCommon::Allocators;

namespace {

const size_t TEST_SIZE = 32;

} // namespace


template <class Allocator>
void TestAllocate()
{
    Memory<Allocator> memory;
    void* p = memory.Allocate(TEST_SIZE);
    ASSERT_NE(nullptr, p);
}

template <class Allocator>
void TestFree()
{
    Memory<Allocator> memory;
    void* p = memory.Allocate(TEST_SIZE);
    ASSERT_NE(nullptr, p);

    memory.Free(p);
}

template <class Allocator>
void TestClear()
{
    Memory<Allocator> memory;
    void* p = memory.Allocate(TEST_SIZE);
    ASSERT_NE(nullptr, p);

    memory.Clear();
}

template <class Allocator>
void TestCollectGarbage()
{
    Memory<Allocator> memory;
    void* p = memory.Allocate(TEST_SIZE);
    ASSERT_NE(nullptr, p);

    memory.CollectGarbage();
}


TEST(Memory, ArenaAllocator_Allocate)
{
    TestAllocate<ArenaAllocator>();
}

TEST(Memory, ArenaAllocator_Free)
{
    TestFree<ArenaAllocator>();
}

TEST(Memory, ArenaAllocator_Clear)
{
    TestClear<ArenaAllocator>();
}

TEST(Memory, ArenaAllocator_CollectGarbage)
{
    TestCollectGarbage<ArenaAllocator>();
}

TEST(Memory, StackAllocator_Allocate)
{
    TestAllocate<StackAllocator<512>>();
}

TEST(Memory, StackAllocator_Free)
{
    TestFree<StackAllocator<512>>();
}

TEST(Memory, StackAllocator_Clear)
{
    TestClear<StackAllocator<512>>();
}

TEST(Memory, StackAllocator_CollectGarbage)
{
    TestCollectGarbage<StackAllocator<512>>();
}
