#pragma once
#define _LKCOMMON_MATH_VECTOR4_HPP_

#include "lkCommon/lkCommon.hpp"

#include <xmmintrin.h>


namespace lkCommon {
namespace Math {

class Vector2;
class Vector3;

/**
 * Zero-overhead 4-dimensional Vector class.
 *
 * Operations on this class are done via SSE instructions to speed up execution time.
 *
 * Casting this function to other Vector* classes will lose values from extra dimensions (ex. cast
 * to Vector2 will only save X and Y coordinates).
 *
 * @todo Document this class properly
 */
class alignas(16) Vector4 final
{
    union Vector4f
    {
        __m128 m;
        float f[4];

        LKCOMMON_INLINE Vector4f()
            : m(_mm_setzero_ps())
        {}

        LKCOMMON_INLINE Vector4f(const __m128& m)
            : m(m)
        {}

        LKCOMMON_INLINE Vector4f(__m128&& m) noexcept
            : m(std::move(m))
        {
        }
    } mValue;

public:
    Vector4();
    Vector4(const float x);
    Vector4(const float x, const float y, const float z, const float w);
    Vector4(const float* v);
    Vector4(const __m128& v);
    Vector4(const Vector4& other);
    Vector4(Vector4&& other) noexcept;
    Vector4& operator=(const Vector4& other);
    Vector4& operator=(Vector4&& other) noexcept;
    ~Vector4() = default;

    const float* Data() const;
    float Length() const;
    Vector4 Normalize() const;

    // Access operator
    float& operator[](const size_t index);
    const float& operator[](const size_t index) const;

    // Addition
    Vector4& operator+=(const Vector4& other);
    Vector4& operator+=(const float value);
    Vector4 operator+(const Vector4& other) const;
    Vector4 operator+(const float value) const;

    // Subtraction
    Vector4& operator-=(const Vector4& other);
    Vector4& operator-=(const float value);
    Vector4 operator-(const Vector4& other) const;
    Vector4 operator-(const float value) const;

    // Multiplication
    Vector4& operator*=(const Vector4& other);
    Vector4& operator*=(const float value);
    Vector4 operator*(const Vector4& other) const;
    Vector4 operator*(const float value) const;

    // Division
    Vector4& operator/=(const Vector4& other);
    Vector4& operator/=(const float value);
    Vector4 operator/(const Vector4& other) const;
    Vector4 operator/(const float value) const;

    // Products
    float Dot(const Vector4& other) const;
    Vector4 Cross(const Vector4& other) const;

    // Comparison
    bool operator==(const Vector4& other) const;
    bool operator!=(const Vector4& other) const;

    // Casts
    explicit operator Vector2() const;
    explicit operator Vector3() const;

    // Friendships
    friend std::ostream& operator<<(std::ostream& os, const Vector4& v);
};

} // namespace Math
} // namespace lkCommon

#include "Vector4Impl.hpp"
