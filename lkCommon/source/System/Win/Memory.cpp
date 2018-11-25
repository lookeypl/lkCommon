#include "lkCommon/System/Memory.hpp"
#include <malloc.h>
#include <Windows.h>


namespace lkCommon {
namespace System {
namespace Memory {

void* AlignedAlloc(size_t size, size_t alignment)
{
    return _aligned_malloc(size, alignment);
}

void AlignedFree(void* ptr)
{
    _aligned_free(ptr);
}

} // namespace lkCommon
} // namespace System
} // namespace Memory
