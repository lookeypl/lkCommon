#pragma once

#ifndef _LKCOMMON_MATH_VECTOR3_HPP_
#error "Please include main header of Vector3, not the implementation header."
#endif // _LKCOMMON_MATH_VECTOR3_HPP_

#include <cmath>


namespace lkCommon {
namespace Math {

LKCOMMON_INLINE Vector3::Vector3()
    : mValue{0.0f, 0.0f, 0.0f}
{
}

LKCOMMON_INLINE Vector3::Vector3(const float x)
    : mValue{x, x, x}
{
}

LKCOMMON_INLINE Vector3::Vector3(const float x, const float y, const float z)
    : mValue{x, y, z}
{
}

LKCOMMON_INLINE Vector3::Vector3(const float* v)
    : mValue{v[0], v[1], v[2]}
{
}

LKCOMMON_INLINE const float* Vector3::Data() const
{
    return mValue;
}

LKCOMMON_INLINE float Vector3::Length() const
{
    return sqrtf(mValue[0] * mValue[0] + mValue[1] * mValue[1] + mValue[2] * mValue[2]);
}

LKCOMMON_INLINE Vector3 Vector3::Normalize() const
{
    const float len = Length();
    return Vector3(*this / len);
}

// Access operator
LKCOMMON_INLINE float& Vector3::operator[](const size_t index)
{
    return mValue[index];
}

LKCOMMON_INLINE const float& Vector3::operator[](const size_t index) const
{
    return mValue[index];
}

// Addition
LKCOMMON_INLINE Vector3& Vector3::operator+=(const Vector3& other)
{
    mValue[0] += other.mValue[0];
    mValue[1] += other.mValue[1];
    mValue[2] += other.mValue[2];

    return *this;
}

LKCOMMON_INLINE Vector3& Vector3::operator+=(const float value)
{
    mValue[0] += value;
    mValue[1] += value;
    mValue[2] += value;

    return *this;
}

LKCOMMON_INLINE Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(
        mValue[0] + other.mValue[0],
        mValue[1] + other.mValue[1],
        mValue[2] + other.mValue[2]
    );
}

LKCOMMON_INLINE Vector3 Vector3::operator+(const float value) const
{
    return Vector3(
        mValue[0] + value,
        mValue[1] + value,
        mValue[2] + value
    );
}

// Subtraction
LKCOMMON_INLINE Vector3& Vector3::operator-=(const Vector3& other)
{
    mValue[0] -= other.mValue[0];
    mValue[1] -= other.mValue[1];
    mValue[2] -= other.mValue[2];

    return *this;
}

LKCOMMON_INLINE Vector3& Vector3::operator-=(const float value)
{
    mValue[0] -= value;
    mValue[1] -= value;
    mValue[2] -= value;

    return *this;
}

LKCOMMON_INLINE Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(
        mValue[0] - other.mValue[0],
        mValue[1] - other.mValue[1],
        mValue[2] - other.mValue[2]
    );
}

LKCOMMON_INLINE Vector3 Vector3::operator-(const float value) const
{
    return Vector3(
        mValue[0] - value,
        mValue[1] - value,
        mValue[2] - value
    );
}

// Multiplication
LKCOMMON_INLINE Vector3& Vector3::operator*=(const Vector3& other)
{
    mValue[0] *= other.mValue[0];
    mValue[1] *= other.mValue[1];
    mValue[2] *= other.mValue[2];

    return *this;
}

LKCOMMON_INLINE Vector3& Vector3::operator*=(const float value)
{
    mValue[0] *= value;
    mValue[1] *= value;
    mValue[2] *= value;

    return *this;
}

LKCOMMON_INLINE Vector3 Vector3::operator*(const Vector3& other) const
{
    return Vector3(
        mValue[0] * other.mValue[0],
        mValue[1] * other.mValue[1],
        mValue[2] * other.mValue[2]
    );
}

LKCOMMON_INLINE Vector3 Vector3::operator*(const float value) const
{
    return Vector3(
        mValue[0] * value,
        mValue[1] * value,
        mValue[2] * value
    );

}

// Division
LKCOMMON_INLINE Vector3& Vector3::operator/=(const Vector3& other)
{
    mValue[0] /= other.mValue[0];
    mValue[1] /= other.mValue[1];
    mValue[2] /= other.mValue[2];

    return *this;
}

LKCOMMON_INLINE Vector3& Vector3::operator/=(const float value)
{
    mValue[0] /= value;
    mValue[1] /= value;
    mValue[2] /= value;

    return *this;
}

LKCOMMON_INLINE Vector3 Vector3::operator/(const Vector3& other) const
{
    return Vector3(
        mValue[0] / other.mValue[0],
        mValue[1] / other.mValue[1],
        mValue[2] / other.mValue[2]
    );
}

LKCOMMON_INLINE Vector3 Vector3::operator/(const float value) const
{
    return Vector3(
        mValue[0] / value,
        mValue[1] / value,
        mValue[2] / value
    );
}

// Products
LKCOMMON_INLINE float Vector3::Dot(const Vector3& other) const
{
    return mValue[0] * other.mValue[0] + mValue[1] * other.mValue[1] + mValue[2] * other.mValue[2];
}

LKCOMMON_INLINE Vector3 Vector3::Cross(const Vector3& other) const
{
    return Vector3(
        mValue[1] * other.mValue[2] - mValue[2] * other.mValue[1],
        mValue[2] * other.mValue[0] - mValue[0] * other.mValue[2],
        mValue[0] * other.mValue[1] - mValue[1] * other.mValue[0]
    );
}

// Comparison
LKCOMMON_INLINE bool Vector3::operator==(const Vector3& other) const
{
    return (
        mValue[0] == other.mValue[0] &&
        mValue[1] == other.mValue[1] &&
        mValue[2] == other.mValue[2]
    );
}

LKCOMMON_INLINE bool Vector3::operator!=(const Vector3& other) const
{
    return (
        mValue[0] != other.mValue[0] ||
        mValue[1] != other.mValue[1] ||
        mValue[2] != other.mValue[2]
    );
}

} // namespace Math
} // namespace lkCommon
