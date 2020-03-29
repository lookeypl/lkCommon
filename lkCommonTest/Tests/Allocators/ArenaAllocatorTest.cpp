#include "lkCommon/Allocators/ArenaAllocator.hpp"
#include "lkCommon/System/Info.hpp"
#include <gtest/gtest.h>
#include <future>

using namespace lkCommon::Allocators;

namespace {

const size_t PAGE_SIZE = lkCommon::System::Info::GetPageSize();
const size_t CUSTOM_CHUNK_SIZE = PAGE_SIZE * 4;
const size_t CUSTOM_CHUNK_SIZE_NOT_PADDED = (PAGE_SIZE * 3) + 1;

const size_t ALLOCATION_SIZE_SMALL = 16;
const size_t THREAD_COUNT = 16;
const uint32_t DEAD_AREA_MAGIC = 0xDEADBEEF;

} // namespace


TEST(ArenaAllocator, Constructor)
{
    ArenaAllocator allocator;
    EXPECT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());
}

TEST(ArenaAllocator, Allocate)
{
    ArenaAllocator allocator;

    ASSERT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());

    EXPECT_NE(nullptr, allocator.Allocate(ALLOCATION_SIZE_SMALL));
    EXPECT_EQ(PAGE_SIZE - ALLOCATION_SIZE_SMALL, allocator.GetFreeChunkSpace());
    EXPECT_EQ(1, allocator.GetChunkCount());
}

TEST(ArenaAllocator, AllocateToChunk)
{
    ArenaAllocator allocator;

    ASSERT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());

    EXPECT_NE(nullptr, allocator.Allocate(PAGE_SIZE));
    EXPECT_EQ(0, allocator.GetFreeChunkSpace());
    EXPECT_EQ(1, allocator.GetChunkCount());
}

TEST(ArenaAllocator, Free)
{
    ArenaAllocator allocator;

    uint32_t* ptr1 = reinterpret_cast<uint32_t*>(
        allocator.Allocate(ALLOCATION_SIZE_SMALL)
    );
    EXPECT_NE(nullptr, ptr1);
    EXPECT_EQ(1, allocator.GetChunkCount());
    memset(ptr1, 0, ALLOCATION_SIZE_SMALL); // in case we hit the same region as in previous run

    uint32_t* ptr2 = reinterpret_cast<uint32_t*>(
        allocator.Allocate(ALLOCATION_SIZE_SMALL)
    );
    EXPECT_NE(nullptr, ptr2);
    EXPECT_EQ(1, allocator.GetChunkCount());
    memset(ptr2, 0, ALLOCATION_SIZE_SMALL); // in case we hit the same region as in previous run

    allocator.Free(ptr1);

    EXPECT_EQ(DEAD_AREA_MAGIC, *ptr1);
    EXPECT_NE(DEAD_AREA_MAGIC, *ptr2);
    EXPECT_EQ(1, allocator.GetChunkCount());
}

TEST(ArenaAllocator, FreeAndAllocate)
{
    ArenaAllocator allocator;

    void* ptr = allocator.Allocate(ALLOCATION_SIZE_SMALL);
    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(1, allocator.GetChunkCount());
    memset(ptr, 0, ALLOCATION_SIZE_SMALL); // in case we hit the same region

    void* ptr2 = ptr;
    allocator.Free(ptr);

    ptr = allocator.Allocate(ALLOCATION_SIZE_SMALL);
    EXPECT_EQ(ptr2, ptr); // Free should reuse the chunk
    EXPECT_EQ(1, allocator.GetChunkCount());
    memset(ptr, 0, ALLOCATION_SIZE_SMALL); // in case we hit the same region
}

TEST(ArenaAllocator, ClearAndAllocate)
{
    ArenaAllocator allocator;

    ASSERT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());

    EXPECT_NE(nullptr, allocator.Allocate(PAGE_SIZE));
    EXPECT_EQ(0, allocator.GetFreeChunkSpace());
    EXPECT_EQ(1, allocator.GetChunkCount());

    allocator.Clear();
    EXPECT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());
    EXPECT_EQ(0, allocator.GetChunkCount());

    EXPECT_NE(nullptr, allocator.Allocate(ALLOCATION_SIZE_SMALL));
    EXPECT_EQ(PAGE_SIZE - ALLOCATION_SIZE_SMALL, allocator.GetFreeChunkSpace());
    EXPECT_EQ(1, allocator.GetChunkCount());
}

TEST(ArenaAllocator, AllocateMultipleThreads)
{
    ArenaAllocator allocator;

    std::vector<void*> memPtrs(THREAD_COUNT);
    std::vector<std::future<void*>> mFutures;
    mFutures.reserve(THREAD_COUNT);

    ASSERT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());

    auto threadFunc = [&allocator]() -> void* {
        return allocator.Allocate(ALLOCATION_SIZE_SMALL);
    };

    for (uint32_t i = 0; i < THREAD_COUNT; ++i)
    {
        mFutures.emplace_back(std::move(std::async(std::launch::async, threadFunc)));
    }

    for (uint32_t i = 0; i < THREAD_COUNT; ++i)
    {
        memPtrs[i] = mFutures[i].get();
        EXPECT_NE(nullptr, memPtrs[i]);
    }

    // ensure addresses did not overlap
    for (uint32_t i = 0; i < THREAD_COUNT; ++i)
    {
        for (uint32_t j = i + 1; j < THREAD_COUNT; ++j)
        {
            EXPECT_NE(memPtrs[i], memPtrs[j]);
        }
    }
}

TEST(ArenaAllocator, AllocateTwoBlocksChunkSize)
{
    ArenaAllocator allocator;

    size_t chunkSize = allocator.GetFreeChunkSpace();

    void* chunk1 = allocator.Allocate(chunkSize);
    ASSERT_NE(nullptr, chunk1);
    ASSERT_EQ(0, allocator.GetFreeChunkSpace());
    EXPECT_EQ(1, allocator.GetChunkCount());


    ASSERT_NE(nullptr, allocator.Allocate(chunkSize));
    ASSERT_NE(0, allocator.GetFreeChunkSpace());
    EXPECT_EQ(2, allocator.GetChunkCount());
}

TEST(ArenaAllocator, AllocateMoreThanChunk)
{
    ArenaAllocator allocator;

    size_t chunkSize = allocator.GetFreeChunkSpace();

    void* ptr = allocator.Allocate(chunkSize + 1);
    ASSERT_NE(nullptr, ptr);

    // we should have double the chunk space, but still one chunk only
    ASSERT_NE(chunkSize * 2, allocator.GetFreeChunkSpace());
    EXPECT_EQ(1, allocator.GetChunkCount());
}

TEST(ArenaAllocator, CollectGarbage)
{
    ArenaAllocator allocator;

    // allocate some data
    size_t chunkSize = allocator.GetFreeChunkSpace();
    const uint32_t CHUNK_COUNT = 5;

    void* ptr[CHUNK_COUNT] = { nullptr, };

    for (uint32_t i = 0; i < CHUNK_COUNT; ++i)
    {
        ptr[i] = allocator.Allocate(chunkSize);
        ASSERT_NE(nullptr, ptr[i]);

        if (i < CHUNK_COUNT - 1)
            chunkSize *= 2;
    }

    // ensure we have multiple chunks
    ASSERT_EQ(CHUNK_COUNT, allocator.GetChunkCount());

    // free data
    for (uint32_t i = 0; i < CHUNK_COUNT; ++i)
    {
        allocator.Free(ptr[i]);
    }

    // we should still have numerous chunks
    ASSERT_EQ(CHUNK_COUNT, allocator.GetChunkCount());

    // go go Gadget's garbage collector
    allocator.CollectGarbage();

    // only one chunk should remain (last, biggest one), totally free
    ASSERT_EQ(1, allocator.GetChunkCount());
    ASSERT_EQ(chunkSize, allocator.GetFreeChunkSpace());
}
