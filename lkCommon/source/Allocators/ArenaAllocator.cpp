#include "lkCommon/Allocators/ArenaAllocator.hpp"
#include "lkCommon/System/Info.hpp"
#include "lkCommon/System/Memory.hpp"
#include "lkCommon/Utils/Logger.hpp"

#include <algorithm>


namespace {

#ifdef _DEBUG
    // Dead area magic is only used in Debug
    const uint32_t DEAD_AREA_MAGIC = 0xDEADBEEF;
#endif // _DEBUG

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
namespace Allocators {

ArenaAllocator::ArenaAllocator()
    : mPageSize(lkCommon::System::Info::GetPageSize())
    , mArenaSize(mPageSize)
    , mArenas()
{
}

ArenaAllocator::~ArenaAllocator()
{
    // free all chunks
    Clear();
}

Arena* ArenaAllocator::AddChunk()
{
    uint8_t* memory = reinterpret_cast<uint8_t*>(System::Memory::AlignedAlloc(mArenaSize, mPageSize));
    if (memory == nullptr)
    {
        LOGE("Failed to allocate aligned block of memory of size " << mArenaSize);
        return nullptr;
    }

    mArenas.emplace_back();
    Arena* arena = &mArenas.back();

    arena->ptr = memory;
    arena->size = arena->sizeLeft = mArenaSize;
    return arena;
}

Arena* ArenaAllocator::FindArenaByPointer(void* ptr)
{
    for (auto& a: mArenas)
    {
        if (a.ptr <= ptr && ptr < (a.ptr + a.size))
            return &a;
    }

    return nullptr;
}

void* ArenaAllocator::Allocate(size_t size)
{
    size = std::max(size, sizeof(uint32_t)); // to ensure we'll be able to fit magic
    Arena* arena;

    if (mArenas.empty())
    {
        // increase the size only if we won't fit size bytes of data
        mArenaSize = EnsureArenaFitsSize(mArenaSize, size);
        arena = AddChunk();
        if (arena == nullptr)
        {
            return nullptr;
        }
    }
    else
    {
        arena = &mArenas.back();
        if (arena->sizeLeft < size)
        {
            mArenaSize *= 2; // double size
            mArenaSize = EnsureArenaFitsSize(mArenaSize, size);
            arena = AddChunk();
            if (arena == nullptr)
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
    Arena* arena = FindArenaByPointer(ptr);
    LKCOMMON_ASSERT(arena != nullptr, "Invalid pointer provided to free");

    #ifdef _DEBUG
        // check if area wasn't freed already and set given memory part as dead
        // in Release this is compiled-out for speed.
        uint32_t* u32ptr = reinterpret_cast<uint32_t*>(ptr);
        LKCOMMON_ASSERT(*u32ptr != DEAD_AREA_MAGIC, "Attempted double-free");
        *u32ptr = DEAD_AREA_MAGIC;
    #endif // _DEBUG

    --arena->referenceCount;
    if (arena->referenceCount == 0)
    {
        // mark arena as free to use again
        arena->sizeLeft = arena->size;
    }
}

void ArenaAllocator::Clear()
{
    if (!mArenas.empty())
    {
        for (auto& c: mArenas)
        {
            System::Memory::AlignedFree(c.ptr);
            c.ptr = nullptr;
        }

        mArenas.clear();
    }
}

void ArenaAllocator::CollectGarbage()
{
    if (mArenas.size() <= 1)
        return;

    ArenaCollection::iterator lastIt = std::prev(mArenas.end());
    ArenaCollection::iterator it = mArenas.begin();

    while(it != lastIt)
    {
        if (it->referenceCount == 0)
            it = mArenas.erase(it);
        else
            ++it;
    }
}

} // namespace Allocators
} // namespace lkCommon
