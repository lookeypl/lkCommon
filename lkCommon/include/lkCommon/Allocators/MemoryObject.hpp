#pragma once

#include "lkCommon/Allocators/Memory.hpp"
#include <cstring>
#include <memory>


namespace lkCommon {
namespace Allocators {

/**
 * Helper template to enforce use of selected memory allocator.
 *
 * This template privatizes default new/delete operators and puts up placement
 * new operators instead, using requrested Allocator. Use this template via
 * inheritance.
 */
template <typename Allocator>
class MemoryObject
{
    // Default new/delete shouldn't return NULL so we have to do this,
    // otherwise some compilers have an issue. We privatize these calls anyway
    // to prevent out-of-allocator allocation so it doesn't matter what's here.
    static void* operator new(size_t) { return LKCOMMON_MEMORY_FREE_AREA; }
    static void operator delete(void*) {}

public:
    LKCOMMON_INLINE static void* operator new(size_t size, Memory<Allocator>& memory)
    {
        return memory.Allocate(size);
    }

    LKCOMMON_INLINE static void operator delete(void* p, Memory<Allocator>& memory)
    {
        memory.Free(p);
    }
};

} // namespace Allocators
} // namespace lkCommon
