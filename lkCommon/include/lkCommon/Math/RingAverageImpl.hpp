#pragma once

#ifndef _LKCOMMON_MATH_RING_AVERAGE_HPP_
#error "Please include main header of RingAverage, not the implementation header."
#endif // _LKCOMMON_MATH_RING_AVERAGE_HPP_


namespace lkCommon {
namespace Math {

template <typename V, size_t N>
RingAverage<V,N>::RingAverage()
    : mValues{0}
    , mCurrentValue(0)
    , mCurrentSize(0)
{
}

template <typename V, size_t N>
V RingAverage<V,N>::Get()
{
    V average = static_cast<V>(0);
    for (size_t i = 0; i < mCurrentSize; ++i)
        average += mValues[i];
    return (average / static_cast<V>(mCurrentSize));
}

template <typename V, size_t N>
void RingAverage<V,N>::Add(V value)
{
    if (mCurrentSize < N)
        mCurrentSize++;

    mCurrentValue++;
    if (mCurrentValue == N)
        mCurrentValue = 0;

    mValues[mCurrentValue] = value;
}

} // namespace Math
} // namespace lkCommon
