#include "lkCommon/Utils/ArenaAllocator.hpp"
#include "lkCommon/System/Info.hpp"
#include "lkCommon/System/Memory.hpp"
#include "lkCommon/Utils/Logger.hpp"

#include <algorithm>


namespace {

const uint32_t DEAD_AREA_MAGIC = 0xDEADBEEF;

LKCOMMON_INLINE size_t EnsureArenaFitsSize(size_t currentArenaSize, size_t toAllocSize)
{
    while (currentArenaSize < toAllocSize)
    {
        currentArenaSize *= 2;
    }

    return currentArenaSize;
}

} // namespace

namespace lkCommon {
namespace Utils {

ArenaAllocator::ArenaAllocator()
    : mPageSize(lkCommon::System::Info::GetPageSize())
    , mArenaSize(mPageSize)
    , mArenas()
    , mAllocatorMutex()
{
}

ArenaAllocator::~ArenaAllocator()
{
    // free all chunks
    FreeChunks();
}

ArenaAllocator::ArenaCollection::iterator ArenaAllocator::AddChunk()
{
    uint8_t* memory = reinterpret_cast<uint8_t*>(System::Memory::AlignedAlloc(mArenaSize, mPageSize));
    if (memory == nullptr)
    {
        LOGE("Failed to allocate aligned block of memory of size " << mArenaSize);
        return mArenas.end();
    }

    mArenas.emplace_back();
    ArenaCollection::iterator arena = --mArenas.end();

    arena->ptr = memory;
    arena->size = arena->sizeLeft = mArenaSize;
    return arena;
}

ArenaAllocator::ArenaCollection::iterator ArenaAllocator::FindFreeArena(size_t size)
{
    for (ArenaCollection::iterator it = mArenas.begin(); it != mArenas.end(); ++it)
    {
        if (it->sizeLeft >= size)
            return it;
    }

    return mArenas.end();
}

ArenaAllocator::ArenaCollection::iterator ArenaAllocator::FindArenaByPointer(void* ptr)
{
    for (ArenaCollection::iterator it = mArenas.begin(); it != mArenas.end(); ++it)
    {
        if (it->ptr <= ptr && ptr < (it->ptr + it->size))
            return it;
    }

    return mArenas.end();
}

void* ArenaAllocator::Allocate(size_t size)
{
    std::lock_guard<std::mutex> allocatorGuard(mAllocatorMutex);

    size = std::max(size, sizeof(uint32_t)); // to ensure we'll be able to fit magic
    ArenaCollection::iterator arena;

    if (mArenas.empty())
    {
        // increase the size only if we won't fit size bytes of data
        mArenaSize = EnsureArenaFitsSize(mArenaSize, size);
        arena = AddChunk();
        if (arena == mArenas.end())
        {
            return nullptr;
        }
    }
    else
    {
        arena = FindFreeArena(size);
        if (arena == mArenas.end())
        {
            mArenaSize *= 2; // double size
            mArenaSize = EnsureArenaFitsSize(mArenaSize, size);
            arena = AddChunk();
            if (arena == mArenas.end())
            {
                return nullptr;
            }
        }
    }

    void* ptr = arena->ptr + (arena->size - arena->sizeLeft);
    arena->sizeLeft -= size;
    ++arena->referenceCount;
    return ptr;
}

void ArenaAllocator::Free(void* ptr)
{
    std::lock_guard<std::mutex> allocatorGuard(mAllocatorMutex);

    ArenaCollection::iterator arena = FindArenaByPointer(ptr);
    LKCOMMON_ASSERT(arena != mArenas.end(), "Invalid pointer provided to free");

    uint32_t* u32ptr = reinterpret_cast<uint32_t*>(ptr);
    LKCOMMON_ASSERT(*u32ptr != DEAD_AREA_MAGIC, "Attempted double-free");
    *u32ptr = DEAD_AREA_MAGIC;

    --arena->referenceCount;
    if (arena->referenceCount == 0)
    {
        System::Memory::AlignedFree(arena->ptr);
        mArenas.erase(arena);
    }
}

void ArenaAllocator::FreeChunks()
{
    std::lock_guard<std::mutex> allocatorGuard(mAllocatorMutex);

    if (!mArenas.empty())
    {
        for (auto& c: mArenas)
        {
            System::Memory::AlignedFree(c.ptr);
        }

        mArenas.clear();
    }
}

} // namespace Utils
} // namespace lkCommon
