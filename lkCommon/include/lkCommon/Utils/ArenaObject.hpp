#pragma once

#include "lkCommon/Utils/ArenaAllocator.hpp"
#include <cstring>


namespace lkCommon {
namespace Utils {

class ArenaObject
{
public:
    void* operator new(size_t size)
    {
        return ArenaAllocator::Instance().Allocate(size);
    }

    void operator delete(void* p)
    {
        return ArenaAllocator::Instance().Free(p);
    }
};

} // namespace lkCommon
} // namespace Utils
