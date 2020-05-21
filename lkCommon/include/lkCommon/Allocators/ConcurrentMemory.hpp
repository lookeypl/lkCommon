#pragma once

#include "lkCommon/lkCommon.hpp"

#include <mutex>
#include <thread>


namespace lkCommon {
namespace Allocators {

/**
 * A thread-safe alternative to Memory template.
 *
 * This class provides an unified interface for all Allocator classes provided
 * by lkCommon library. For even more simplicity & ease-of-use, classes can
 * derive after ConcurrentMemoryObject.
 *
 * If need occurs, all used memory can be freed by using Clear() function.
 *
 * On some Allocators it's worth to call CollectGarbage() function regularly
 * to clean some leftover, unused data blocks.
 *
 * This template serves both as a passthrough and automatically locks a mutex
 * when entering thread-sensitive functions (Allocate and Free). Other
 * functions are assumed to be called by main thread so they are NOT
 * thread-safe.
 *
 * @sa Memory
 * @sa ConcurrentMemoryObject
 * @sa ArenaAllocator
 */
template <typename Allocator>
class ConcurrentMemory
{
    Allocator mAllocator;
    std::mutex mMutex;

public:
    /**
     * Allocate @p size amount of bytes on given Allocator.
     *
     * @p[in] size Size of data to allocate in bytes.
     * @return Pointer to allocated data, nullptr on error.
     */
    LKCOMMON_INLINE void* Allocate(size_t size)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        return mAllocator.Allocate(size);
    }

    /**
     * Free @p size amount of bytes from given Allocator.
     *
     * @p[in] ptr Pointer to data to free
     */
    LKCOMMON_INLINE void Free(void* ptr)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mAllocator.Free(ptr);
    }

    /**
     * Completely clear contents of used Allocator.
     *
     * This can free any memory which was allocated to perform @f Allocate
     * calls. Accessing such memory after Clear() is an undefined behavior.
     */
    LKCOMMON_INLINE void Clear()
    {
        mAllocator.Clear();
    }

    /**
     * Perform garbage collection routine in used Allocator.
     *
     * Some Allocators might allocate multiple pools of data in order to keep
     * up with current memory demands. On some Allocators this call can be
     * used to return any unused memory resources back to the System.
     *
     * Each CollectGarbage() call should provide minimal possible overhead
     * when there's no garbage to clean.
     */
    LKCOMMON_INLINE void CollectGarbage()
    {
        mAllocator.CollectGarbage();
    }
};

} // namespace Allocators
} // namespace lkCommon
