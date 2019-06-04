#pragma once
#define _LKCOMMON_MATH_MATRIX4_HPP_

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Math/Vector4.hpp>
#include <xmmintrin.h>


namespace lkCommon {
namespace Math {

/**
 * Class representing a zero-overhead 4x4 matrix, calculated using SSE instructions.
 *
 * @note Matrix is constructed as row-major
 */
class LKCOMMON_ALIGN(16) Matrix4 final
{
    union Matrix4f
    {
        Vector4 v[4];
        __m128 m[4];
        float f[16];

        LKCOMMON_INLINE Matrix4f()
            : m{ _mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps() }
        {
        }

        LKCOMMON_INLINE Matrix4f(const __m128& a, const __m128& b, const __m128& c, const __m128& d)
            : m{ a, b, c, d }
        {
        }

        LKCOMMON_INLINE Matrix4f(__m128&& a, __m128&& b, __m128&& c, __m128&& d)
            : m{ std::move(a), std::move(b), std::move(c), std::move(d) }
        {
        }

        LKCOMMON_INLINE Matrix4f(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d)
            : v{ a, b, c, d }
        {
        }

        LKCOMMON_INLINE Matrix4f(Vector4&& a, Vector4&& b, Vector4&& c, Vector4&& d)
            : v{ std::move(a), std::move(b), std::move(c), std::move(d) }
        {
        }
    } v;

public:
    Matrix4();
    Matrix4(const float a);
    Matrix4(const float* a);
    Matrix4(const float a1, const float a2, const float a3, const float a4,
            const float b1, const float b2, const float b3, const float b4,
            const float c1, const float c2, const float c3, const float c4,
            const float d1, const float d2, const float d3, const float d4);
    Matrix4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d);
    Matrix4(const __m128& a, const __m128& b, const __m128& c, const __m128& d);
    Matrix4(const Matrix4& other);
    Matrix4(Matrix4&& other) noexcept;
    Matrix4& operator=(const Matrix4& other);
    Matrix4& operator=(Matrix4&& other) noexcept;
    ~Matrix4() = default;

    const float* Data() const;

    // Access operators
    Vector4& operator[](const size_t index);
    const Vector4& operator[](const size_t index) const;

    // Addition
    Matrix4& operator+=(const Matrix4& other);
    Matrix4& operator+=(const float value);
    Matrix4 operator+(const Matrix4& other) const;
    Matrix4 operator+(const float value) const;

    // Subtraction
    Matrix4& operator-=(const Matrix4& other);
    Matrix4& operator-=(const float value);
    Matrix4 operator-(const Matrix4& other) const;
    Matrix4 operator-(const float value) const;

    // Multiplication
    Matrix4& operator*=(const Matrix4& other);
    Matrix4& operator*=(const float value);
    Matrix4 operator*(const Matrix4& other) const;
    Matrix4 operator*(const float value) const;

    // Division
    Matrix4& operator/=(const float value);
    Matrix4 operator/(const float value) const;

    // Comparison
    bool operator==(const Matrix4& other) const;
    bool operator!=(const Matrix4& other) const;

    // Operations
    Matrix4 Transpose() const;

    // Friendships
    friend std::ostream& operator<<(std::ostream& os, const Matrix4& v);
    friend Vector4 operator*(const Matrix4& a, const Vector4& b);

    // Matrix4 creators
    static Matrix4 CreateRHLookAt(const Vector4& pos, const Vector4& dir, const Vector4& up);
    static Matrix4 CreateRHProjection(const float fov, const float aspectRatio, const float nearDist, const float farDist);
    static Matrix4 CreateRotationX(const float angle);
    static Matrix4 CreateRotationY(const float angle);
    static Matrix4 CreateRotationZ(const float angle);
    static Matrix4 CreateTranslation(const Vector4& translation);
    static Matrix4 CreateScale(const float scale);
    static Matrix4 CreateScale(const float scaleX, const float scaleY, const float scaleZ);
    static Matrix4 CreateScale(const Vector4& scale);

    // Constants
    static const Matrix4 IDENTITY; ///< Identity Matrix4 constant.
};

// External operators (cross-type)
Vector4 operator*(const Matrix4& a, const Vector4& b);


} // namespace Math
} // namespace lkCommon

#include "Matrix4Impl.hpp"
