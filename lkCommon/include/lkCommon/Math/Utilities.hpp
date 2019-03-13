#pragma once
#define _LKCOMMON_MATH_UTILITIES_HPP_

#include "lkCommon/Math/Vector4.hpp"


namespace lkCommon {
namespace Math {
namespace Util {

/**
 * Linearly interpolate between two objects.
 *
 * @note For this template to compile, object must have defined arithmetic operators +, - and *
 *       between two objects of type T and between object T and float.
 *
 * @p[in] a      Object of type T to interpolate from
 * @p[in] b      Object of type T to interpolate to
 * @p[in] factor Factor of interpolation
 * @result Interpolated value of type T
 */
template <typename T>
T Lerp(const T& a, const T& b, const float factor);

/**
 * Rotate vector "mimicking" the rotation between two other vectors, using Rodrigues' rotation formula.
 *
 * @p[in] v    Vector to rotate
 * @p[in] from Source vector used to form a rotation
 * @p[in] to   Destination vector used to form a rotation
 * @result Vector @p v rotated in same way as @p from was rotated to @p to
 */
Vector4 RotateFromToVector(const Vector4& v, const Vector4& from, const Vector4& to);

/**
 * Randomly cosine-sample a hemisphere, given two random RNG numbers.
 *
 * @p[in] normal Hemisphere's normal vector
 * @p[in] x1     Random number 1, should be in range 0..1
 * @p[in] x2     Random number 2, should be in range 0..1
 * @result Random vector from hemisphere, cosine-weighted
 *
 * Sampling works by selecting a random point on a disk with normal vector pointing towards Z axis,
 * which is at hemisphere's base. Then, this point is casted onto a hemisphere, a direction is
 * formed and rotated to align it to provided @p normal.
 */
Vector4 CosineSampleHemisphere(const Vector4& normal, const float x1, const float x2);

/**
 * Randomly cosine-sample a hemisphere.
 *
 * @p[in] normal Hemisphere's normal vector
 * @result Random vector from hemisphere, cosine-weighted
 *
 * Function will randomly generate two numbers using Xorshift algorithm and use them to cosine-sample
 * a hemisphere. For more details on sampling, check overload with RNG numbers in argument.
 */
Vector4 CosineSampleHemisphere(const Vector4& normal);

} // namespace Util
} // namespace Math
} // namespace lkCommon

#include "UtilitiesImpl.hpp"
