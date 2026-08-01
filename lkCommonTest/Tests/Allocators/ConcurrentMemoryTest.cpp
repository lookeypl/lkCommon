#include <lkCommon/Allocators/ConcurrentMemory.hpp>
#include <lkCommon/Allocators/ArenaAllocator.hpp>
#include <lkCommon/System/Info.hpp>
#include <gtest/gtest.h>

#include <future>


using namespace lkCommon::Allocators;

namespace {

const size_t ALLOCATION_SIZE_SMALL = 16;
const size_t THREAD_COUNT = 16;

} // namespace


TEST(ConcurrentMemory, AllocateMultipleThreads)
{
    ConcurrentMemory<ArenaAllocator> allocator;

    std::vector<void*> memPtrs(THREAD_COUNT);
    std::vector<std::future<void*>> mFutures;
    mFutures.reserve(THREAD_COUNT);

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
