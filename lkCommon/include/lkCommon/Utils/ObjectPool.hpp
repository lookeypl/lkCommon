#pragma once
#define _LKCOMMON_UTILS_OBJECTPOOL_HPP_

#include "lkCommon/Utils/StaticStack.hpp"

#include <vector>
#include <list>
#include <numeric>
#include <type_traits>


namespace lkCommon {
namespace Utils {

/**
 * A pool of objects providing fast allocation.
 *
 * ObjectPool allocates chunks of memory which fit a predefined amount of objects, thus speeding
 * up subsequent allocations. At the same time, limiting the size of performed allocations to only
 * one type of object allows ObjectPool to easier keep track of freed objects for reuse.
 *
 * Currently ObjectPool assumes uint8_t objects in one chunk.
 */
template <class T>
class ObjectPool
{
    struct Chunk
    {
        lkCommon::Utils::StaticStack<uint8_t, 256> freed;
        T objects[256];
    };

    std::vector<uint8_t> mUsedSlots;
    std::list<Chunk> mChunks;

    void AllocateNewChunk();

public:
    ObjectPool();

    T* Allocate();

    void Free();
};

} // namespace Utils
} // namespace lkCommon


#include "ObjectPoolImpl.hpp"
