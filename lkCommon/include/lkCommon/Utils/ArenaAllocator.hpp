#pragma once

#include "lkCommon/lkCommon.hpp"

#include <list>
#include <thread>
#include <mutex>


namespace lkCommon {
namespace Utils {

struct MemoryChunk
{
    uint8_t* ptr;
    size_t sizeLeft;

    MemoryChunk()
        : ptr(nullptr)
        , sizeLeft(0)
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
 * For speed it is assumed that objects in ArenaAllocator cannot be singularly freed. The only way
 * to empty used memory is to free all the chunks, which will invalidate all allocated data.
 *
 * @note Allocated chunks are aligned to system's page size.
 */
class ArenaAllocator
{
    size_t mPageSize;
    size_t mChunkSize;
    std::list<MemoryChunk> mChunks;
    std::mutex mAllocatorMutex;

    bool AddChunk();

public:
    /**
     * Create an ArenaAllocator with default chunk size equal to system's page size.
     */
    ArenaAllocator();

    /**
     * Create an ArenaAllocator with custom base chunk size @p baseChunkSize.
     *
     * @p[in] baseChunkSize Size of first chunk in bytes.
     *
     * @remarks Because memory is aligned to single page size, use @p baseChunkSize equal to, or
     *          multiple of system's chunk size. If other size is used, baseChunkSize is
     *          automatically padded to next chunk size fitting baseChunkSize bytes of memory.
     *
     * @sa lkCommon::System::Info::GetPageSize
     */
    ArenaAllocator(size_t baseChunkSize);

    /**
     * Destroy an ArenaAllocator. All chunks will be freed, which also frees all allocated memory.
     *
     * All references to memory which was acquired from @f Allocate() will become invalid and
     * accessing them will result in undefined behavior.
     */
    ~ArenaAllocator();

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
        if (mChunks.empty())
            return mChunkSize;
        else
            return mChunks.back().sizeLeft;
    }
};

} // namespace Utils
} // namespace lkCommon
