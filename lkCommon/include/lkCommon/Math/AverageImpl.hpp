#pragma once

#ifndef _LKCOMMON_MATH_AVERAGE_HPP_
#error "Please include main header of Average, not the implementation header."
#endif // _LKCOMMON_MATH_AVERAGE_HPP_

#include "lkCommon/lkCommon.hpp"

namespace lkCommon {
namespace Math {

template <typename V>
Average<V>::Average()
    : mSum(static_cast<V>(0))
    , mValueCount(0)
    , mMutex()
{
}

template <typename V>
LKCOMMON_INLINE V Average<V>::Get() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mSum / mValueCount;
}

template <typename V>
LKCOMMON_INLINE void Average<V>::Add(V value)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mSum += value;
    mValueCount++;
}

template <typename V>
LKCOMMON_INLINE void Average<V>::Clear()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mSum = static_cast<V>(0);
    mValueCount = 0;
}

} // namespace Math
} // namespace lkCommon

#include "AverageImpl.hpp"
