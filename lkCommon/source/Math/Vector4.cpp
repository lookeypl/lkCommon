#include "lkCommon/Math/Vector4.hpp"

#include <smmintrin.h>
#include <cmath>


namespace lkCommon {
namespace Math {

Vector4::Vector4()
{
    value.m = _mm_setzero_ps();
}

Vector4::Vector4(const float x)
{
    value.m = _mm_set_ps1(x);
}

Vector4::Vector4(const float x, const float y, const float z, const float w)
{
    value.m = _mm_set_ps(w, z, y, x);
}

Vector4::Vector4(const float* v)
{
   value.m = _mm_load_ps(v);
}

Vector4::Vector4(__m128 v)
{
    value.m = v;
}

Vector4::Vector4(const Vector4& other)
{
    value.m = _mm_load_ps(other.value.f);
}

Vector4::Vector4(Vector4&& other)
{
    value.m = _mm_load_ps(other.value.f);
}

Vector4& Vector4::operator=(const Vector4& other)
{
    value.m = _mm_load_ps(other.value.f);
    return *this;
}

Vector4& Vector4::operator=(Vector4&& other)
{
    value.m = _mm_load_ps(other.value.f);
    return *this;
}

Vector4::~Vector4()
{
}

const float* Vector4::Data() const
{
    return value.f;
}

float Vector4::Length() const
{
    return sqrt(value.f[0] * value.f[0] + value.f[1] * value.f[1] + value.f[2] * value.f[2] + value.f[3] * value.f[3]);
}

void Vector4::Normalize()
{
    float coeff = 1.0f / Length();
    *this *= coeff;
}

// Access operator
float Vector4::operator[](int index) const
{
    return value.f[index];
}

// Addition
Vector4& Vector4::operator+=(const Vector4& other)
{
    value.m = _mm_add_ps(value.m, other.value.m);
    return *this;
}

Vector4& Vector4::operator+=(const float value)
{
    __m128 v = _mm_set_ps1(value);
    this->value.m = _mm_add_ps(this->value.m, v);
    return *this;
}

const Vector4 Vector4::operator+(const Vector4& other) const
{
    return Vector4(*this) += other;
}

const Vector4 Vector4::operator+(const float value) const
{
    return Vector4(*this) += value;
}

// Subtraction
Vector4& Vector4::operator-=(const Vector4& other)
{
    value.m = _mm_sub_ps(value.m, other.value.m);
    return *this;
}

Vector4& Vector4::operator-=(const float value)
{
    __m128 v = _mm_set_ps1(value);
    this->value.m = _mm_sub_ps(this->value.m, v);
    return *this;
}

const Vector4 Vector4::operator-(const Vector4& other) const
{
    return Vector4(*this) -= other;
}

const Vector4 Vector4::operator-(const float value) const
{
    return Vector4(*this) -= value;
}

// Multiplication
Vector4& Vector4::operator*=(const Vector4& other)
{
    value.m = _mm_mul_ps(value.m, other.value.m);
    return *this;
}

Vector4& Vector4::operator*=(const float value)
{
    __m128 v = _mm_set_ps1(value);
    this->value.m = _mm_mul_ps(this->value.m, v);
    return *this;
}

const Vector4 Vector4::operator*(const Vector4& other) const
{
    return Vector4(*this) *= other;
}

const Vector4 Vector4::operator*(const float value) const
{
    return Vector4(*this) *= value;
}

// Division
Vector4& Vector4::operator/=(const Vector4& other)
{
    value.m = _mm_div_ps(value.m, other.value.m);
    return *this;
}

Vector4& Vector4::operator/=(const float value)
{
    __m128 v = _mm_set_ps1(value);
    this->value.m = _mm_div_ps(this->value.m, v);
    return *this;
}

const Vector4 Vector4::operator/(const Vector4& other) const
{
    return Vector4(*this) /= other;
}

const Vector4 Vector4::operator/(const float value) const
{
    return Vector4(*this) /= value;
}

// Products
float Vector4::Dot(const Vector4& other)
{
    Vector4f result;
    result.m = _mm_dp_ps(value.m, other.value.m, 0xF1);
    return result.f[0];
}

Vector4& Vector4::Cross(const Vector4& other)
{
    __m128 test = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
    __m128 test2 = _mm_shuffle_ps(test, test, _MM_SHUFFLE(3, 1, 0, 2));

    __m128 shufa1 = _mm_shuffle_ps(value.m, value.m, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 shufb1 = _mm_shuffle_ps(other.value.m, other.value.m, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 shufa2 = _mm_shuffle_ps(value.m, value.m, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 shufb2 = _mm_shuffle_ps(other.value.m, other.value.m, _MM_SHUFFLE(3, 0, 2, 1));

    __m128 mul1 = _mm_mul_ps(shufa1, shufb1);
    __m128 mul2 = _mm_mul_ps(shufa2, shufb2);
    value.m = _mm_sub_ps(mul1, mul2);
    return *this;
}

// Comparison
bool Vector4::operator==(const Vector4& other) const
{
    Vector4f res;
    res.m = _mm_cmpeq_ps(value.m, other.value.m);
    return res.f[0] && res.f[1] && res.f[2] && res.f[3];
}

bool Vector4::operator!=(const Vector4& other) const
{
    Vector4f res;
    res.m = _mm_cmpneq_ps(value.m, other.value.m);
    return res.f[0] || res.f[1] || res.f[2] || res.f[3];
}

// Friendships
std::ostream& operator<<(std::ostream& os, const Vector4& v)
{
    os << "[" << v.value.f[0] << ", " << v.value.f[1] << ", " << v.value.f[2] << ", " << v.value.f[3] << "]";
    return os;
}

} // namespace Math
} // namespace lkCommon
