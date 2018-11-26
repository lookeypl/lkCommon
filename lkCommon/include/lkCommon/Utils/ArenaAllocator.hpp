#pragma once

#include "lkCommon/lkCommon.hpp"

#include <list>
#include <thread>
#include <mutex>


namespace lkCommon {
namespace Utils {

struct Arena
{
    uint8_t* ptr;
    size_t size;
    size_t sizeLeft;
    size_t referenceCount;

    Arena()
        : ptr(nullptr)
        , size(0)
        , sizeLeft(0)
        , referenceCount(0)
    {
    }
};

/**
 * Allocates chunks of memory in system and provides interface to allocate data on it.
 *
 * ArenaAllocator's goal is to reduce allocations done through system calls. Object manages chunks
 * of data allocated in heap and distributes them to user. As a result, for lots of small objects
 * number of system memory allocation calls is greatly reduced, which should result in speedup.
 *
 * At first Allocate() call, ArenaAllocator allocates one chunk of memory of @p mChunkSize size. If,
 * after numerous Allocate() calls there's not enough space, mChunkSize is increased by single page
 * size and new chunk is added to collection.
 *
 * It is assumed that objects in ArenaAllocator can be singularly freed, however this will only
 * decrease reference counter for given chunk. Such memory won't be possible to reclaim,
 *
 * If need occurs, all used memory can be freed by
 * to empty used memory is to free all the chunks, which will invalidate all allocated data.
 *
 * @note Allocated arenas are aligned to system's page size.
 */
class ArenaAllocator
{
    using ArenaCollection = std::list<Arena>;

    size_t mPageSize;
    size_t mArenaSize;
    ArenaCollection mArenas;
    std::mutex mAllocatorMutex;

    ArenaCollection::iterator AddChunk();
    ArenaCollection::iterator FindFreeArena(size_t size);
    ArenaCollection::iterator FindArenaByPointer(void* ptr);

    /**
     * Create an ArenaAllocator with default chunk size equal to system's page size.
     */
    ArenaAllocator();

    /**
     * Destroy an ArenaAllocator. All chunks will be freed, which also frees all allocated memory.
     *
     * All references to memory which was acquired from @f Allocate() will become invalid and
     * accessing them will result in undefined behavior.
     */
    ~ArenaAllocator();

    ArenaAllocator(const ArenaAllocator&) = delete;
    ArenaAllocator(ArenaAllocator&&) = delete;
    ArenaAllocator operator=(const ArenaAllocator&) = delete;
    ArenaAllocator& operator=(ArenaAllocator&&) = delete;

public:
    /**
     * Allocator instance acquisitor.
     *
     * @return Instance of current ArenaAllocator.
     */
    static ArenaAllocator& Instance()
    {
        static ArenaAllocator instance;
        return instance;
    }

    /**
     * Allocate data of size @p size and return pointer to it.
     *
     * @p[in] size Size of memory to allocate in the arena in bytes.
     *
     * Function might allocate a new chunk of memory in following conditions:
     *   - There's not enough space in currently used chunk
     *   - Size of allocated data exceeds current chunk size
     *
     * In such case, all currently allocated blocks remain in old chunk and all future allocations
     * will be directed to new chunk with doubled size.
     *
     * For best performance, allocated chunks should have way smaller size than chunk size. If
     * there will be an allocation of size higher than current chunk size, Allocator will increase
     * the chunk size to fit an object of given size, plus padding necessary to align to page size.
     *
     * This call is thread-safe.
     */
    void* Allocate(size_t size);

    /**
     * Free pointer from Arena.
     *
     * @p ptr Pointer to object to free.
     *
     * This function will find chunk to which pointer belongs and decrease allocated object count
     * at that chunk. If chunk has zero allocations, it will be marked for reuse.
     */
    void Free(void* ptr);

    /**
     * Frees all chunks. After this call all allocated data will be invalid.
     *
     * It is caller's duty to ensure that all data will not be used after this point.
     *
     * After this call object is still usable and Allocate() can be called again for
     * new chunk allocation.
     */
    void FreeChunks();

    /**
     * Returns free space in currently active chunk of data.
     *
     * @warning This function is for test purposes only. In real life scenarios, ArenaAllocator
     *          will add new chunks when needed. As a result, this function does NOT reflect total
     *          free space available for use.
     */
    LKCOMMON_INLINE size_t GetFreeChunkSpace() const
    {
        if (mArenas.empty())
            return mArenaSize;
        else
            return mArenas.back().sizeLeft;
    }
};

} // namespace Utils
} // namespace lkCommon
