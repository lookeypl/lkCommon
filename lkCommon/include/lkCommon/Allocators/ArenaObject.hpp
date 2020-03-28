#pragma once

#include "lkCommon/Utils/ArenaAllocator.hpp"
#include <cstring>
#include <memory>


namespace lkCommon {
namespace Allocators {

class ArenaObject
{
    static void* operator new(size_t size) { return malloc(size); };
    static void operator delete(void* p) { free(p); };

public:
    static void* operator new(size_t size, ArenaAllocator& allocator)
    {
        return allocator.Allocate(size);
    }

    static void operator delete(void* p, ArenaAllocator& allocator)
    {
        allocator.Free(p);
    }
};

} // namespace Allocators
} // namespace lkCommon
