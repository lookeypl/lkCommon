#pragma once

namespace lkCommon {
namespace Math {
namespace Random {

/**
 * Get a random number using Xorshift algorithm
 *
 * @p[inout] seed Seed used to randomize a number. Will be updated to new value after randomization.
 * @return Random float from 0 to 1 range
 */
float Xorshift(uint32_t& state);

/**
 * Get a random number using Xorshift algorithm
 *
 * @return Random float from 0 to 1 range
 */
float Xorshift();

} // namespace Random
} // namespace Math
} // namespace lkCommon

#include "RandomImpl.hpp"
