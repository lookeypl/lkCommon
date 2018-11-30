#pragma once

#include "lkCommon/Utils/ArenaAllocator.hpp"
#include <cstring>
#include <memory>


namespace lkCommon {
namespace Utils {

class ArenaObject
{
    static void* operator new(size_t size);
    static void operator delete(void* p);

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

} // namespace lkCommon
} // namespace Utils
