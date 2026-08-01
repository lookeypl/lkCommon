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
    static void* operator new(size_t) { return nullptr; }
    static void operator delete(void*) { }

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
