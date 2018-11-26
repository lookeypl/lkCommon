#include "lkCommon/Utils/ArenaAllocator.hpp"
#include "lkCommon/System/Info.hpp"
#include <gtest/gtest.h>
#include <future>

using namespace lkCommon::Utils;

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
    ArenaAllocator::Instance().FreeChunks();

    EXPECT_EQ(PAGE_SIZE, ArenaAllocator::Instance().GetFreeChunkSpace());
}

TEST(ArenaAllocator, Allocate)
{
    ArenaAllocator::Instance().FreeChunks();

    ASSERT_EQ(PAGE_SIZE, ArenaAllocator::Instance().GetFreeChunkSpace());

    EXPECT_NE(nullptr, ArenaAllocator::Instance().Allocate(ALLOCATION_SIZE_SMALL));
    EXPECT_EQ(PAGE_SIZE - ALLOCATION_SIZE_SMALL, ArenaAllocator::Instance().GetFreeChunkSpace());
}

TEST(ArenaAllocator, AllocateToChunk)
{
    ArenaAllocator::Instance().FreeChunks();

    ASSERT_EQ(PAGE_SIZE, ArenaAllocator::Instance().GetFreeChunkSpace());

    EXPECT_NE(nullptr, ArenaAllocator::Instance().Allocate(PAGE_SIZE));
    EXPECT_EQ(0, ArenaAllocator::Instance().GetFreeChunkSpace());
}

TEST(ArenaAllocator, Free)
{
    ArenaAllocator::Instance().FreeChunks();

    uint32_t* ptr1 = reinterpret_cast<uint32_t*>(
        ArenaAllocator::Instance().Allocate(ALLOCATION_SIZE_SMALL)
    );
    EXPECT_NE(nullptr, ptr1);
    memset(ptr1, 0, ALLOCATION_SIZE_SMALL); // in case we hit the same region

    uint32_t* ptr2 = reinterpret_cast<uint32_t*>(
        ArenaAllocator::Instance().Allocate(ALLOCATION_SIZE_SMALL)
    );
    EXPECT_NE(nullptr, ptr2);
    memset(ptr2, 0, ALLOCATION_SIZE_SMALL); // in case we hit the same region

    ArenaAllocator::Instance().Free(ptr1);

    EXPECT_EQ(DEAD_AREA_MAGIC, *ptr1);
    EXPECT_NE(DEAD_AREA_MAGIC, *ptr2);
}

TEST(ArenaAllocator, FreeAndAllocate)
{
    ArenaAllocator::Instance().FreeChunks();

    void* ptr = ArenaAllocator::Instance().Allocate(ALLOCATION_SIZE_SMALL);
    EXPECT_NE(nullptr, ptr);
    memset(ptr, 0, ALLOCATION_SIZE_SMALL); // in case we hit the same region

    ArenaAllocator::Instance().Free(ptr);

    ptr = ArenaAllocator::Instance().Allocate(ALLOCATION_SIZE_SMALL);
    EXPECT_NE(nullptr, ptr);
    memset(ptr, 0, ALLOCATION_SIZE_SMALL); // in case we hit the same region
}

TEST(ArenaAllocator, FreeChunksAndAllocate)
{
    ArenaAllocator::Instance().FreeChunks();

    ASSERT_EQ(PAGE_SIZE, ArenaAllocator::Instance().GetFreeChunkSpace());

    EXPECT_NE(nullptr, ArenaAllocator::Instance().Allocate(PAGE_SIZE));
    EXPECT_EQ(0, ArenaAllocator::Instance().GetFreeChunkSpace());

    ArenaAllocator::Instance().FreeChunks();
    EXPECT_EQ(PAGE_SIZE, ArenaAllocator::Instance().GetFreeChunkSpace());

    EXPECT_NE(nullptr, ArenaAllocator::Instance().Allocate(ALLOCATION_SIZE_SMALL));
    EXPECT_EQ(PAGE_SIZE - ALLOCATION_SIZE_SMALL, ArenaAllocator::Instance().GetFreeChunkSpace());
}

TEST(ArenaAllocator, AllocateMultipleThreads)
{
    ArenaAllocator::Instance().FreeChunks();

    std::vector<void*> memPtrs(THREAD_COUNT);
    std::vector<std::future<void*>> mFutures;
    mFutures.reserve(THREAD_COUNT);

    ASSERT_EQ(PAGE_SIZE, ArenaAllocator::Instance().GetFreeChunkSpace());

    auto threadFunc = []() -> void* {
        return ArenaAllocator::Instance().Allocate(ALLOCATION_SIZE_SMALL);
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
    ArenaAllocator::Instance().FreeChunks();

    size_t chunkSize = ArenaAllocator::Instance().GetFreeChunkSpace();

    void* chunk1 = ArenaAllocator::Instance().Allocate(chunkSize);
    ASSERT_NE(nullptr, chunk1);
    ASSERT_EQ(0, ArenaAllocator::Instance().GetFreeChunkSpace());

    ASSERT_NE(nullptr, ArenaAllocator::Instance().Allocate(chunkSize));
    ASSERT_NE(0, ArenaAllocator::Instance().GetFreeChunkSpace());
}

TEST(ArenaAllocator, AllocateMoreThanChunk)
{
    ArenaAllocator::Instance().FreeChunks();

    size_t chunkSize = ArenaAllocator::Instance().GetFreeChunkSpace();

    void* chunk1 = ArenaAllocator::Instance().Allocate(chunkSize + 1);
    ASSERT_NE(nullptr, chunk1);
    ASSERT_NE(0, ArenaAllocator::Instance().GetFreeChunkSpace());
}
