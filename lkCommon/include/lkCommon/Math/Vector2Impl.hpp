#pragma once

#ifndef _LKCOMMON_MATH_VECTOR2_HPP_
#error "Please include main header of Vector2, not the implementation header."
#endif // _LKCOMMON_MATH_VECTOR2_HPP_

#include <cmath>


namespace lkCommon {
namespace Math {

LKCOMMON_INLINE Vector2::Vector2()
    : mValue{0.0f, 0.0f}
{
}

LKCOMMON_INLINE Vector2::Vector2(const float x)
    : mValue{x, x}
{
}

LKCOMMON_INLINE Vector2::Vector2(const float x, const float y)
    : mValue{x, y}
{
}

LKCOMMON_INLINE Vector2::Vector2(const float* v)
    : mValue{v[0], v[1]}
{
}

LKCOMMON_INLINE const float* Vector2::Data() const
{
    return mValue;
}

LKCOMMON_INLINE float Vector2::Length() const
{
    return sqrtf(mValue[0] * mValue[0] + mValue[1] * mValue[1]);
}

LKCOMMON_INLINE Vector2 Vector2::Normalize() const
{
    const float len = Length();
    return Vector2(*this / len);
}

// Access operator
LKCOMMON_INLINE float& Vector2::operator[](const size_t index)
{
    return mValue[index];
}

LKCOMMON_INLINE const float& Vector2::operator[](const size_t index) const
{
    return mValue[index];
}

// Addition
LKCOMMON_INLINE Vector2& Vector2::operator+=(const Vector2& other)
{
    mValue[0] += other.mValue[0];
    mValue[1] += other.mValue[1];

    return *this;
}

LKCOMMON_INLINE Vector2& Vector2::operator+=(const float value)
{
    mValue[0] += value;
    mValue[1] += value;

    return *this;
}

LKCOMMON_INLINE Vector2 Vector2::operator+(const Vector2& other) const
{
    return Vector2(
        mValue[0] + other.mValue[0],
        mValue[1] + other.mValue[1]
    );
}

LKCOMMON_INLINE Vector2 Vector2::operator+(const float value) const
{
    return Vector2(
        mValue[0] + value,
        mValue[1] + value
    );
}

// Subtraction
LKCOMMON_INLINE Vector2& Vector2::operator-=(const Vector2& other)
{
    mValue[0] -= other.mValue[0];
    mValue[1] -= other.mValue[1];

    return *this;
}

LKCOMMON_INLINE Vector2& Vector2::operator-=(const float value)
{
    mValue[0] -= value;
    mValue[1] -= value;

    return *this;
}

LKCOMMON_INLINE Vector2 Vector2::operator-(const Vector2& other) const
{
    return Vector2(
        mValue[0] - other.mValue[0],
        mValue[1] - other.mValue[1]
    );
}

LKCOMMON_INLINE Vector2 Vector2::operator-(const float value) const
{
    return Vector2(
        mValue[0] - value,
        mValue[1] - value
    );
}

// Multiplication
LKCOMMON_INLINE Vector2& Vector2::operator*=(const Vector2& other)
{
    mValue[0] *= other.mValue[0];
    mValue[1] *= other.mValue[1];

    return *this;
}

LKCOMMON_INLINE Vector2& Vector2::operator*=(const float value)
{
    mValue[0] *= value;
    mValue[1] *= value;

    return *this;
}

LKCOMMON_INLINE Vector2 Vector2::operator*(const Vector2& other) const
{
    return Vector2(
        mValue[0] * other.mValue[0],
        mValue[1] * other.mValue[1]
    );
}

LKCOMMON_INLINE Vector2 Vector2::operator*(const float value) const
{
    return Vector2(
        mValue[0] * value,
        mValue[1] * value
    );

}

// Division
LKCOMMON_INLINE Vector2& Vector2::operator/=(const Vector2& other)
{
    mValue[0] /= other.mValue[0];
    mValue[1] /= other.mValue[1];

    return *this;
}

LKCOMMON_INLINE Vector2& Vector2::operator/=(const float value)
{
    mValue[0] /= value;
    mValue[1] /= value;

    return *this;
}

LKCOMMON_INLINE Vector2 Vector2::operator/(const Vector2& other) const
{
    return Vector2(
        mValue[0] / other.mValue[0],
        mValue[1] / other.mValue[1]
    );
}

LKCOMMON_INLINE Vector2 Vector2::operator/(const float value) const
{
    return Vector2(
        mValue[0] / value,
        mValue[1] / value
    );
}

// Products
LKCOMMON_INLINE float Vector2::Dot(const Vector2& other) const
{
    return mValue[0] * other.mValue[0] + mValue[1] * other.mValue[1];
}

// Comparison
LKCOMMON_INLINE bool Vector2::operator==(const Vector2& other) const
{
    return (
        mValue[0] == other.mValue[0] &&
        mValue[1] == other.mValue[1]
    );
}

LKCOMMON_INLINE bool Vector2::operator!=(const Vector2& other) const
{
    return (
        mValue[0] != other.mValue[0] ||
        mValue[1] != other.mValue[1]
    );
}

} // namespace Math
} // namespace lkCommon
