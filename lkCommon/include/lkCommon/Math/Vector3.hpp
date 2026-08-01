#pragma once
#define _LKCOMMON_MATH_VECTOR3_HPP_

#include "lkCommon/lkCommon.hpp"


namespace lkCommon {
namespace Math {

class Vector2;
class Vector4;

/**
 * Zero-overhead 3-dimensional vector class.
 *
 * Class has the same interface as Vector4. The only difference is lack of SSE
 * support. SSE implementation would force 16-byte object size, which in some
 * cases (ex. storing an array of veritces) might do more harm than good. For
 * computationally-heavy parts of code use Vector4 class for faster execution
 * time.
 *
 * Casting to higher-dimension Vector4 object assumes extra dimensions equal to
 * zero. Casting to lower-dimension Vector2 class loses extra dimensions (only
 * X and Y coordinates are taken into account).
 *
 * @todo Document this class properly
 */
class Vector3 final
{
    float mValue[3];

public:
    Vector3();
    Vector3(const float x);
    Vector3(const float x, const float y, const float z);
    Vector3(const float* v);
    Vector3(const Vector3& other) = default;
    Vector3(Vector3&& other) = default;
    Vector3& operator=(const Vector3& other) = default;
    Vector3& operator=(Vector3&& other) = default;
    ~Vector3() = default;

    const float* Data() const;
    float Length() const;
    Vector3 Normalize() const;

    // Access operator
    float& operator[](const size_t index);
    const float& operator[](const size_t index) const;

    // Addition
    Vector3& operator+=(const Vector3& other);
    Vector3& operator+=(const float value);
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator+(const float value) const;

    // Subtraction
    Vector3& operator-=(const Vector3& other);
    Vector3& operator-=(const float value);
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator-(const float value) const;

    // Multiplication
    Vector3& operator*=(const Vector3& other);
    Vector3& operator*=(const float value);
    Vector3 operator*(const Vector3& other) const;
    Vector3 operator*(const float value) const;

    // Division
    Vector3& operator/=(const Vector3& other);
    Vector3& operator/=(const float value);
    Vector3 operator/(const Vector3& other) const;
    Vector3 operator/(const float value) const;

    // Products
    float Dot(const Vector3& other) const;
    Vector3 Cross(const Vector3& other) const;

    // Comparison
    bool operator==(const Vector3& other) const;
    bool operator!=(const Vector3& other) const;

    // Casts
    explicit operator Vector2() const;
    explicit operator Vector4() const;

    // Friendships
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
};

} // namespace Math
} // namespace lkCommon

#include "Vector3Impl.hpp"
