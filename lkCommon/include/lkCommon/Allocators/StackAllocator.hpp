#pragma once

#include "lkCommon/lkCommon.hpp"


namespace lkCommon {
namespace Allocators {

/**
 * Allocates chunks of memory on a stack-based memory region.
 *
 * @p MemorySize Size of available memory in bytes.
 *
 * Provides a simpler and faster allocation than @ref ArenaAllocator and lack of malloc/free calls
 * at a cost of less flexibility and freedom of use:
 *   - Max available size is limited by @p MemorySize and cannot be increased in runtime
 *   - Allocator has to be entirely cleaned to reuse already allocated space
 *   - There is no way to free specific chunks of memory
 */
template<size_t MemorySize>
class StackAllocator
{
    size_t mUsed;
    uint8_t mMemory[MemorySize];

public:
    /**
     * Create a StackAllocator
     */
    StackAllocator()
        : mUsed(0)
    {
    }

    /**
     * Destroy a StackAllocator
     */
    ~StackAllocator()
    {
    }

    StackAllocator(const StackAllocator&) = delete;
    StackAllocator(StackAllocator&&) = delete;
    StackAllocator operator=(const StackAllocator&) = delete;
    StackAllocator& operator=(StackAllocator&&) = delete;

    /**
     * Allocate data of size @p size and return pointer to it.
     *
     * @p[in] size Size of memory to allocate on stack in bytes.
     *
     * Allocator provides no mechanism for supporting arbitrary size of allocated memory. Exceeding
     * available memory size is provided only via an assertion, so it is compiled out on Release
     * configuration. It is application's duty to ensure allocations are not going to perform out
     * of bounds writes or reads on returned memory chunk.
     *
     * For simplicity and performance reasons it is impossible to free allocated pointers. Used
     * memory can be reclaimed via @ref Clear() call.
     */
    LKCOMMON_INLINE void* Allocate(size_t size)
    {
        LKCOMMON_ASSERT(size <= (MemorySize - mUsed), "Not enough space");
        void* ptr = &mMemory[mUsed];
        mUsed += size;
        return ptr;
    }

    /**
     * Stub implementation - this allocator does not support freeing singular chunks of allocated
     * memory.
     */
    LKCOMMON_INLINE void Free(void* ptr)
    {
        LKCOMMON_UNUSED(ptr);
        // nothing to do
    }

    /**
     * Clear the allocator, reclaiming all allocated memory for reuse.
     *
     * In fact, this call just zeroes the "used memory" variable. The entire memory block remains
     * intact, however it can now be overwritten with new allocations. It is best to assume, that
     * after this call all pointers acquired from @ref Allocate() are invalid.
     */
    LKCOMMON_INLINE void Clear()
    {
        mUsed = 0;
    }

    /**
     * Stub implementation - this allocator does not have a need for garbage collection.
     */
    LKCOMMON_INLINE void CollectGarbage()
    {
        // nothing to do
    }

    /**
     * Returns amount of bytes currently allocated via the allocator.
     */
    LKCOMMON_INLINE size_t GetUsedMemory() const
    {
        return mUsed;
    }
};

} // namespace Allocators
} // namespace lkCommon
