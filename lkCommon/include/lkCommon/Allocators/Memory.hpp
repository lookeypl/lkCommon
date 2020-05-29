#pragma once

#include "lkCommon/lkCommon.hpp"


namespace lkCommon {
namespace Allocators {

/**
 * A Memory allocated by custom lkCommon Allocator.
 *
 * This class provides an unified interface for all Allocator classes provided
 * by lkCommon library. For even more simplicity & ease-of-use, classes can
 * derive after MemoryObject.
 *
 * If need occurs, all used memory can be freed by using Clear() function.
 *
 * On some Allocators it's worth to call CollectGarbage() function regularly
 * to clean some leftover, unused data blocks.
 *
 * This template only serves as a passthrough and provides no extra
 * functionalities. For thread-safe version of Memory, see ConcurrentMemory.
 *
 * @sa ConcurrentMemory
 * @sa MemoryObject
 * @sa ArenaAllocator
 */
template <typename Allocator>
class Memory
{
    Allocator mAllocator;

public:
    /**
     * Allocate @p size amount of bytes on given Allocator.
     *
     * @p[in] size Size of data to allocate in bytes.
     * @return Pointer to allocated data, nullptr on error.
     */
    LKCOMMON_INLINE void* Allocate(size_t size)
    {
        return mAllocator.Allocate(size);
    }

    /**
     * Free @p size amount of bytes from given Allocator.
     *
     * @p[in] ptr Pointer to data to free
     */
    LKCOMMON_INLINE void Free(void* ptr)
    {
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
