#include "lkCommon/System/Memory.hpp"
#include <stdlib.h>
#include <algorithm>


namespace lkCommon {
namespace System {
namespace Memory {

void* AlignedAlloc(size_t size, size_t alignment)
{
    size_t voidPtrSize = sizeof(void*);
    size = ((size % voidPtrSize) == 0) ? size : size + (voidPtrSize - (size % voidPtrSize));
    void* ptr = nullptr;
    int ret = posix_memalign(&ptr, size, alignment);
    if (ret != 0)
        return nullptr;
    return ptr;
}

void AlignedFree(void* ptr)
{
    free(ptr);
}

} // namespace lkCommon
} // namespace System
} // namespace Memory
