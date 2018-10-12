#pragma once

#include <array>

namespace lkCommon {
namespace Math {

// Calculates an average from count values
template <typename V, size_t N>
class RingAverage
{
    std::array<V, N> mValues;
    size_t mCurrentValue;
    size_t mCurrentSize;

public:
    RingAverage();

    V Get();
    void Add(V value);
};

} // namespace Math
} // namespace lkCommon

#include "RingAverageImpl.hpp"
