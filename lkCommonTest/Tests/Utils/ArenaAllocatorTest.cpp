#include "lkCommon/Utils/ArenaAllocator.hpp"
#include "lkCommon/System/Info.hpp"
#include <gtest/gtest.h>
#include <future>

using namespace lkCommon::Utils;

const size_t PAGE_SIZE = lkCommon::System::Info::GetPageSize();
const size_t CUSTOM_CHUNK_SIZE = PAGE_SIZE * 4;
const size_t CUSTOM_CHUNK_SIZE_NOT_PADDED = (PAGE_SIZE * 3) + 1;

const size_t ALLOCATION_SIZE_SMALL = 16;
const size_t THREAD_COUNT = 16;

TEST(ArenaAllocator, Constructor)
{
    ArenaAllocator allocator;
    EXPECT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());
}

TEST(ArenaAllocator, ConstructorCustomChunkSize)
{
    ArenaAllocator allocator(CUSTOM_CHUNK_SIZE);
    EXPECT_EQ(CUSTOM_CHUNK_SIZE, allocator.GetFreeChunkSpace());
}

TEST(ArenaAllocator, ConstructorCustomChunkSizeNotPadded)
{
    ArenaAllocator allocator(CUSTOM_CHUNK_SIZE_NOT_PADDED);
    // constructor should ceil chunk size to nearest multiple of page size
    EXPECT_EQ(CUSTOM_CHUNK_SIZE, allocator.GetFreeChunkSpace());
}

TEST(ArenaAllocator, Allocate)
{
    ArenaAllocator allocator;

    ASSERT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());

    EXPECT_NE(nullptr, allocator.Allocate(ALLOCATION_SIZE_SMALL));
    EXPECT_EQ(PAGE_SIZE - ALLOCATION_SIZE_SMALL, allocator.GetFreeChunkSpace());
}

TEST(ArenaAllocator, AllocateToChunk)
{
    ArenaAllocator allocator;

    ASSERT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());

    EXPECT_NE(nullptr, allocator.Allocate(PAGE_SIZE));
    EXPECT_EQ(0, allocator.GetFreeChunkSpace());
}

TEST(ArenaAllocator, FreeAndAllocate)
{
    ArenaAllocator allocator;

    ASSERT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());

    EXPECT_NE(nullptr, allocator.Allocate(PAGE_SIZE));
    EXPECT_EQ(0, allocator.GetFreeChunkSpace());

    allocator.FreeChunks();
    EXPECT_EQ(PAGE_SIZE, allocator.GetFreeChunkSpace());

    EXPECT_NE(nullptr, allocator.Allocate(ALLOCATION_SIZE_SMALL));
    EXPECT_EQ(PAGE_SIZE - ALLOCATION_SIZE_SMALL, allocator.GetFreeChunkSpace());
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
