#pragma once

#include <cstring>


namespace lkCommon {
namespace System {
namespace Memory {

/**
 * Allocates aligned memory block.
 *
 * @p[in] size      Size of memory to allocate.
 * @p[in] alignment Alignment to use.
 *
 * @return Pointer to allocated memory, nullptr if allocation failed (not enough memory).
 */
void* AlignedAlloc(size_t size, size_t alignment);

/**
 * Frees aligned memory block.
 *
 * @p[in] ptr Block to free.
 */
void AlignedFree(void* ptr);

} // namespace lkCommon
} // namespace System
} // namespace Memory
