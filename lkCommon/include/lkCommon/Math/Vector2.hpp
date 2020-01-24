#pragma once
#define _LKCOMMON_MATH_VECTOR2_HPP_

#include "lkCommon/lkCommon.hpp"


namespace lkCommon {
namespace Math {

class Vector3;
class Vector4;

/**
 * Zero-overhead 2-dimensional vector class.
 *
 * Class has the same interface as Vector4. The only difference is lack of SSE
 * support. SSE implementation would force 16-byte object size, which in some
 * cases (ex. storing an array of veritces) might do more harm than good. For
 * computationally-heavy parts of code use Vector4 class for faster execution
 * time.
 *
 * Casting to higher-dimension vectors (ex. Vector3, Vector4) assumes extra
 * dimensions equal to zero.
 *
 * @todo Document this class properly
 */
class Vector2 final
{
    float mValue[2];

public:
    Vector2();
    Vector2(const float x);
    Vector2(const float x, const float y);
    Vector2(const float* v);
    Vector2(const Vector2& other) = default;
    Vector2(Vector2&& other) = default;
    Vector2& operator=(const Vector2& other) = default;
    Vector2& operator=(Vector2&& other) = default;
    ~Vector2() = default;

    const float* Data() const;
    float Length() const;
    Vector2 Normalize() const;

    // Access operator
    float& operator[](const size_t index);
    const float& operator[](const size_t index) const;

    // Addition
    Vector2& operator+=(const Vector2& other);
    Vector2& operator+=(const float value);
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator+(const float value) const;

    // Subtraction
    Vector2& operator-=(const Vector2& other);
    Vector2& operator-=(const float value);
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator-(const float value) const;

    // Multiplication
    Vector2& operator*=(const Vector2& other);
    Vector2& operator*=(const float value);
    Vector2 operator*(const Vector2& other) const;
    Vector2 operator*(const float value) const;

    // Division
    Vector2& operator/=(const Vector2& other);
    Vector2& operator/=(const float value);
    Vector2 operator/(const Vector2& other) const;
    Vector2 operator/(const float value) const;

    // Products
    float Dot(const Vector2& other) const;

    // Comparison
    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;

    // Casts
    explicit operator Vector3() const;
    explicit operator Vector4() const;

    // Friendships
    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);
};

} // namespace Math
} // namespace lkCommon

#include "Vector2Impl.hpp"
