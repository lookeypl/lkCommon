#include "lkCommon/Utils/ArenaAllocator.hpp"
#include "lkCommon/System/Info.hpp"
#include "lkCommon/System/Memory.hpp"
#include "lkCommon/Utils/Logger.hpp"


namespace lkCommon {
namespace Utils {

ArenaAllocator::ArenaAllocator()
    : mPageSize(lkCommon::System::Info::GetPageSize())
    , mChunkSize(mPageSize)
    , mChunks()
    , mAllocatorMutex()
{
}

ArenaAllocator::ArenaAllocator(size_t baseChunkSize)
    : mPageSize(lkCommon::System::Info::GetPageSize())
    , mChunkSize(baseChunkSize)
    , mChunks()
    , mAllocatorMutex()
{
    size_t modulo = mChunkSize % mPageSize;

    if (modulo != 0)
    {
        LOGW("Requested chunk size is " << mChunkSize << ", while page size is " << lkCommon::System::Info::GetPageSize() << ".");

        mChunkSize = mChunkSize + (mPageSize - modulo);
        LOGW("Initial chunk size for ArenaAllocator was not a multiple of system's page size - padding automatically to " <<
             mChunkSize << " bytes.");
    }
}

ArenaAllocator::~ArenaAllocator()
{
    // free all chunks
    FreeChunks();
}

bool ArenaAllocator::AddChunk()
{
    mChunks.emplace_back();
    MemoryChunk& newChunk = mChunks.back();

    newChunk.ptr = reinterpret_cast<uint8_t*>(System::Memory::AlignedAlloc(mChunkSize, mPageSize));
    if (newChunk.ptr == nullptr)
    {
        LOGE("Failed to allocate aligned block of memory");
        mChunks.pop_back();
        return false;
    }

    newChunk.sizeLeft = mChunkSize;
    return true;
}

void* ArenaAllocator::Allocate(size_t size)
{
    std::lock_guard<std::mutex> allocatorGuard(mAllocatorMutex);

    if (mChunks.empty() || mChunks.back().sizeLeft < size)
    {
        if (!mChunks.empty())
        {
            // increase chunk size only when we already have some data...
            mChunkSize += mPageSize;
        }

        // ...but, nevertheless, increase the size if we won't fit size bytes of data
        while (mChunkSize < size)
        {
            mChunkSize += mPageSize;
        }

        if (!AddChunk())
        {
            return nullptr;
        }
    }

    MemoryChunk& chunk = mChunks.back();

    void* ptr = chunk.ptr + chunk.sizeLeft;
    chunk.sizeLeft -= size;
    return ptr;
}

void ArenaAllocator::FreeChunks()
{
    for (auto& c: mChunks)
    {
        System::Memory::AlignedFree(c.ptr);
    }

    mChunks.clear();
}

} // namespace Utils
} // namespace lkCommon
