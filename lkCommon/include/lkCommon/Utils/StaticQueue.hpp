#pragma once
#define _LKCOMMON_UTILS_STATIC_QUEUE_HPP_

#include "lkCommon/lkCommon.hpp"


namespace lkCommon {
namespace Utils {

/**
 * A lightweight FIFO queue implementation with zero dynamic allocations.
 *
 * @warning For performance, this queue does not perform any checks on
 *          Release code. All error checks are done with asserts, which are
 *          removed from code on Release build.
 */
template <typename T, size_t N>
class StaticQueue
{
    T mObjects[N];
    size_t mStart;
    size_t mEnd;
    bool mWrapped;

public:
    StaticQueue();
    ~StaticQueue() = default;

    /**
     * Copy an object to a queue.
     *
     * @p[in] object Object to be copied on a queue.
     */
    void Push(const T& object);

    /**
     * Move an object to a queue.
     *
     * @p[in] object Object to be moved on a queue.
     */
    void Push(T&& object);

    /**
     * Construct an object and put it on a queue.
     *
     * @p[in] args Arguments forwarded to Queue's constructor.
     */
    template <typename... Args>
    void Emplace(Args&&... args);

    /**
     * Move an element from queue
     */
    T&& Pop();

    /**
     * Acquire queue's max possible size.
     */
    constexpr size_t Capacity() const
    {
        return N;
    }

    /**
     * Acquire amount of elements already on queue.
     */
    LKCOMMON_INLINE size_t Size() const
    {
        if (mWrapped)
            return N - mStart + mEnd;
        else
            return mEnd - mStart;
    }

    LKCOMMON_INLINE bool Empty() const
    {
        return (mStart == mEnd);
    }
};

} // namespace Utils
} // namespace lkCommon


#include "StaticQueueImpl.hpp"
