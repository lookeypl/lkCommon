#pragma once

#ifndef _LKCOMMON_MATH_VECTOR4_HPP_
#error "Please include main header of Vector4, not the implementation header."
#endif // _LKCOMMON_MATH_VECTOR4_HPP_

#include "lkCommon/Math/Random.hpp"
#include <smmintrin.h>
#include <cmath>


namespace lkCommon {
namespace Math {

LKCOMMON_INLINE Vector4::Vector4()
    : mValue()
{
}

LKCOMMON_INLINE Vector4::Vector4(const float x)
    : mValue(_mm_set_ps1(x))
{
}

LKCOMMON_INLINE Vector4::Vector4(const float x, const float y, const float z, const float w)
    : mValue(_mm_set_ps(w, z, y, x))
{
}

LKCOMMON_INLINE Vector4::Vector4(const float* v)
    : mValue(_mm_load_ps(v))
{
}

LKCOMMON_INLINE Vector4::Vector4(const __m128& v)
    : mValue(v)
{
}

LKCOMMON_INLINE Vector4::Vector4(const Vector4& other)
    : mValue(other.mValue.m)
{
}

LKCOMMON_INLINE Vector4::Vector4(Vector4&& other) noexcept
    : mValue(std::move(other.mValue.m))
{
}

LKCOMMON_INLINE Vector4& Vector4::operator=(const Vector4& other)
{
    mValue.m = other.mValue.m;
    return *this;
}

LKCOMMON_INLINE Vector4& Vector4::operator=(Vector4&& other) noexcept
{
    mValue.m = other.mValue.m;
    return *this;
}

LKCOMMON_INLINE const float* Vector4::Data() const
{
    return mValue.f;
}

LKCOMMON_INLINE float Vector4::Length() const
{
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(mValue.m, mValue.m, 0xF1)));
}

LKCOMMON_INLINE Vector4 Vector4::Normalize() const
{
    return _mm_div_ps(mValue.m, _mm_sqrt_ps(_mm_dp_ps(mValue.m, mValue.m, 0x7F)));
}

// Access operator
LKCOMMON_INLINE float& Vector4::operator[](const size_t index)
{
    return mValue.f[index];
}

LKCOMMON_INLINE const float& Vector4::operator[](const size_t index) const
{
    return mValue.f[index];
}

// Addition
LKCOMMON_INLINE Vector4& Vector4::operator+=(const Vector4& other)
{
    mValue.m = _mm_add_ps(mValue.m, other.mValue.m);
    return *this;
}

LKCOMMON_INLINE Vector4& Vector4::operator+=(const float value)
{
    mValue.m = _mm_add_ps(mValue.m, _mm_set_ps1(value));
    return *this;
}

LKCOMMON_INLINE Vector4 Vector4::operator+(const Vector4& other) const
{
    return Vector4(_mm_add_ps(mValue.m, other.mValue.m));
}

LKCOMMON_INLINE Vector4 Vector4::operator+(const float value) const
{
    return Vector4(_mm_add_ps(mValue.m, _mm_set_ps1(value)));
}

// Subtraction
LKCOMMON_INLINE Vector4& Vector4::operator-=(const Vector4& other)
{
    mValue.m = _mm_sub_ps(mValue.m, other.mValue.m);
    return *this;
}

LKCOMMON_INLINE Vector4& Vector4::operator-=(const float value)
{
    mValue.m = _mm_sub_ps(mValue.m, _mm_set_ps1(value));
    return *this;
}

LKCOMMON_INLINE Vector4 Vector4::operator-(const Vector4& other) const
{
    return Vector4(_mm_sub_ps(mValue.m, other.mValue.m));
}

LKCOMMON_INLINE Vector4 Vector4::operator-(const float value) const
{
    return Vector4(_mm_sub_ps(mValue.m, _mm_set_ps1(value)));
}

// Multiplication
LKCOMMON_INLINE Vector4& Vector4::operator*=(const Vector4& other)
{
    mValue.m = _mm_mul_ps(mValue.m, other.mValue.m);
    return *this;
}

LKCOMMON_INLINE Vector4& Vector4::operator*=(const float value)
{
    mValue.m = _mm_mul_ps(mValue.m, _mm_set_ps1(value));
    return *this;
}

LKCOMMON_INLINE Vector4 Vector4::operator*(const Vector4& other) const
{
    return Vector4(_mm_mul_ps(mValue.m, other.mValue.m));
}

LKCOMMON_INLINE Vector4 Vector4::operator*(const float value) const
{
    return Vector4(_mm_mul_ps(mValue.m, _mm_set_ps1(value)));

}

// Division
LKCOMMON_INLINE Vector4& Vector4::operator/=(const Vector4& other)
{
    mValue.m = _mm_div_ps(mValue.m, other.mValue.m);
    return *this;
}

LKCOMMON_INLINE Vector4& Vector4::operator/=(const float value)
{
    mValue.m = _mm_div_ps(mValue.m, _mm_set_ps1(value));
    return *this;
}

LKCOMMON_INLINE Vector4 Vector4::operator/(const Vector4& other) const
{
    return Vector4(_mm_div_ps(mValue.m, other.mValue.m));
}

LKCOMMON_INLINE Vector4 Vector4::operator/(const float value) const
{
    return Vector4(_mm_div_ps(mValue.m, _mm_set_ps1(value)));
}

// Products
LKCOMMON_INLINE float Vector4::Dot(const Vector4& other) const
{
    return _mm_cvtss_f32(_mm_dp_ps(mValue.m, other.mValue.m, 0xF1));
}

LKCOMMON_INLINE Vector4 Vector4::Cross(const Vector4& other) const
{
    return Vector4(_mm_sub_ps(
        _mm_mul_ps(
            _mm_shuffle_ps(mValue.m, mValue.m, _MM_SHUFFLE(3, 0, 2, 1)),
            _mm_shuffle_ps(other.mValue.m, other.mValue.m, _MM_SHUFFLE(3, 1, 0, 2))
        ),
        _mm_mul_ps(
            _mm_shuffle_ps(mValue.m, mValue.m, _MM_SHUFFLE(3, 1, 0, 2)),
            _mm_shuffle_ps(other.mValue.m, other.mValue.m, _MM_SHUFFLE(3, 0, 2, 1))
        )
    ));
}

// Comparison
LKCOMMON_INLINE bool Vector4::operator==(const Vector4& other) const
{
    return (_mm_movemask_ps(_mm_cmpeq_ps(mValue.m, other.mValue.m)) == 0xF);
}

LKCOMMON_INLINE bool Vector4::operator!=(const Vector4& other) const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(mValue.m, other.mValue.m)) != 0);
}

} // namespace Math
} // namespace lkCommon
