#pragma once

#include "lkCommon/lkCommon.hpp"
#include "lkCommon/Math/Constants.hpp"
#include "lkCommon/Math/Random.hpp"
#include <cstdint>
#include <climits>

namespace lkCommon {
namespace Math {
namespace Random {

LKCOMMON_INLINE float Xorshift(uint32_t& state)
{
    state ^= (state << 13);
    state ^= (state >> 17);
    state ^= (state << 5);
    return static_cast<float>(state) / static_cast<float>(UINT32_MAX);
}

LKCOMMON_INLINE float Xorshift()
{
    static uint32_t state = LKCOMMON_XORSHIFT_INITIAL_STATE;
    return Xorshift(state);
}

} // namespace Random
} // namespace Math
} // namespace lkCommon

