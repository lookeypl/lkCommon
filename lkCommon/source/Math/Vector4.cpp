#include "lkCommon/Math/Vector4.hpp"

#include <smmintrin.h>
#include <cmath>


namespace lkCommon {
namespace Math {

Vector4::Vector4()
    : mValue()
{
}

Vector4::Vector4(const float x)
    : mValue(_mm_set_ps1(x))
{
}

Vector4::Vector4(const float x, const float y, const float z, const float w)
    : mValue(_mm_set_ps(w, z, y, x))
{
}

Vector4::Vector4(const float* v)
    : mValue(_mm_load_ps(v))
{
}

Vector4::Vector4(const __m128& v)
    : mValue(v)
{
}

Vector4::Vector4(const Vector4& other)
    : mValue(other.mValue.m)
{
}

Vector4::Vector4(Vector4&& other)
    : mValue(other.mValue.m)
{
}

Vector4& Vector4::operator=(const Vector4& other)
{
    mValue.m = other.mValue.m;
    return *this;
}

Vector4& Vector4::operator=(Vector4&& other)
{
    mValue.m = other.mValue.m;
    return *this;
}

const float* Vector4::Data() const
{
    return mValue.f;
}

float Vector4::Length() const
{
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(mValue.m, mValue.m, 0xF1)));
}

Vector4 Vector4::Normalize() const
{
    return Vector4(_mm_mul_ps(mValue.m, _mm_rsqrt_ps(_mm_dp_ps(mValue.m, mValue.m, 0x7F))));
}

// Access operator
float Vector4::operator[](int index) const
{
    return mValue.f[index];
}

// Addition
Vector4& Vector4::operator+=(const Vector4& other)
{
    mValue.m = _mm_add_ps(mValue.m, other.mValue.m);
    return *this;
}

Vector4& Vector4::operator+=(const float value)
{
    mValue.m = _mm_add_ps(mValue.m, _mm_set_ps1(value));
    return *this;
}

Vector4 Vector4::operator+(const Vector4& other) const
{
    return Vector4(_mm_add_ps(mValue.m, other.mValue.m));
}

Vector4 Vector4::operator+(const float value) const
{
    return Vector4(_mm_add_ps(mValue.m, _mm_set_ps1(value)));
}

// Subtraction
Vector4& Vector4::operator-=(const Vector4& other)
{
    mValue.m = _mm_sub_ps(mValue.m, other.mValue.m);
    return *this;
}

Vector4& Vector4::operator-=(const float value)
{
    mValue.m = _mm_sub_ps(mValue.m, _mm_set_ps1(value));
    return *this;
}

Vector4 Vector4::operator-(const Vector4& other) const
{
    return Vector4(_mm_sub_ps(mValue.m, other.mValue.m));
}

Vector4 Vector4::operator-(const float value) const
{
    return Vector4(_mm_sub_ps(mValue.m, _mm_set_ps1(value)));
}

// Multiplication
Vector4& Vector4::operator*=(const Vector4& other)
{
    mValue.m = _mm_mul_ps(mValue.m, other.mValue.m);
    return *this;
}

Vector4& Vector4::operator*=(const float value)
{
    mValue.m = _mm_mul_ps(mValue.m, _mm_set_ps1(value));
    return *this;
}

Vector4 Vector4::operator*(const Vector4& other) const
{
    return Vector4(_mm_mul_ps(mValue.m, other.mValue.m));
}

Vector4 Vector4::operator*(const float value) const
{
    return Vector4(_mm_mul_ps(mValue.m, _mm_set_ps1(value)));

}

// Division
Vector4& Vector4::operator/=(const Vector4& other)
{
    mValue.m = _mm_div_ps(mValue.m, other.mValue.m);
    return *this;
}

Vector4& Vector4::operator/=(const float value)
{
    mValue.m = _mm_div_ps(mValue.m, _mm_set_ps1(value));
    return *this;
}

Vector4 Vector4::operator/(const Vector4& other) const
{
    return Vector4(_mm_div_ps(mValue.m, other.mValue.m));
}

Vector4 Vector4::operator/(const float value) const
{
    return Vector4(_mm_div_ps(mValue.m, _mm_set_ps1(value)));
}

// Products
float Vector4::Dot(const Vector4& other) const
{
    return _mm_cvtss_f32(_mm_dp_ps(mValue.m, other.mValue.m, 0xF1));
}

Vector4 Vector4::Cross(const Vector4& other) const
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
bool Vector4::operator==(const Vector4& other) const
{
    return (_mm_movemask_ps(_mm_cmpeq_ps(mValue.m, other.mValue.m)) == 0xF);
}

bool Vector4::operator!=(const Vector4& other) const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(mValue.m, other.mValue.m)) != 0);
}

// Friendships
std::ostream& operator<<(std::ostream& os, const Vector4& v)
{
    os << "[" << v.mValue.f[0] << ", " << v.mValue.f[1] << ", " << v.mValue.f[2] << ", " << v.mValue.f[3] << "]";
    return os;
}

} // namespace Math
} // namespace lkCommon
