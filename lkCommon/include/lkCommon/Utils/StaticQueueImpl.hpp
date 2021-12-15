#pragma once

#ifndef _LKCOMMON_UTILS_STATIC_QUEUE_HPP_
#error "Please include main header of StaticQueue, not the implementation header."
#endif // _LKCOMMON_UTILS_STATIC_QUEUE_HPP_

#include "StaticQueue.hpp"


namespace lkCommon {
namespace Utils {

template <typename T, size_t N>
StaticQueue<T, N>::StaticQueue()
    : mObjects()
    , mStart(0)
    , mEnd(0)
    , mWrapped(false)
{
}

template <typename T, size_t N>
void StaticQueue<T, N>::Push(const T& object)
{
    LKCOMMON_ASSERT(Size() < Capacity(), "Queue full");
    ++mEnd;

    if (mEnd == N)
    {
        mEnd = 0;
        mWrapped = true;
        mObjects[N - 1] = object;
        return;
    }

    mObjects[mEnd - 1] = object;
}

template <typename T, size_t N>
void StaticQueue<T, N>::Push(T&& object)
{
    LKCOMMON_ASSERT(Size() < Capacity(), "Queue full");
    ++mEnd;

    if (mEnd == N)
    {
        mEnd = 0;
        mWrapped = true;
        mObjects[N - 1] = std::move(object);
        return;
    }

    mObjects[mEnd - 1] = std::move(object);
}

template <typename T, size_t N>
template <typename... Args>
void StaticQueue<T, N>::Emplace(Args&&... args)
{
    LKCOMMON_ASSERT(Size() < Capacity(), "Queue full");
    ++mEnd;

    if (mEnd == N)
    {
        mEnd = 0;
        mWrapped = true;
        mObjects[N - 1] = T(std::forward<Args>(args)...);
        return;
    }

    mObjects[mEnd - 1] = T(std::forward<Args>(args)...);
}

template <typename T, size_t N>
T&& StaticQueue<T, N>::Pop()
{
    LKCOMMON_ASSERT(Size() != 0, "Queue empty");
    ++mStart;

    if (mStart == N)
    {
        mStart = 0;
        mWrapped = false;
        return std::move(mObjects[N - 1]);
    }

    return std::move(mObjects[mStart - 1]);
}

} // namespace Utils
} // namespace lkCommon
