#pragma once

#ifndef _LKCOMMON_UTILS_PIXEL_HPP_
#error "Please include main header of Pixel, not the implementation header."
#endif // _LKCOMMON_UTILS_PIXEL_HPP_

#include "Pixel.hpp"

#include <cmath>


namespace {


// value range clamps per type (ex. limits float to 0.0f - 1.0f range)

LKCOMMON_INLINE uint8_t Clamp(uint8_t x)
{
    return x;
}

LKCOMMON_INLINE float Clamp(float x)
{
    x = (x > 1.0f ? 1.0f : x);
    return (x < 0.0f ? 0.0f : x);
}


// converters

LKCOMMON_INLINE uint8_t ConvertColor(float source)
{
    return static_cast<uint8_t>(Clamp(source) * 255);
}

LKCOMMON_INLINE float ConvertColor(uint8_t source)
{
    return static_cast<float>(Clamp(source)) / 255.0f;
}


} // namespace


namespace lkCommon {
namespace Utils {


// General template definition

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>::Pixel()
    : mColors{ static_cast<T>(0) }
{
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>::Pixel(const T& color)
{
    for (size_t i = 0; i < ComponentCount; ++i)
        mColors[i] = color;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>::Pixel(const T colors[ComponentCount])
{
    for (size_t i = 0; i < ComponentCount; ++i)
        mColors[i] = colors[i];
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>::Pixel(const std::initializer_list<T>& l)
    : mColors{ static_cast<T>(0) }
{
    size_t limit = ComponentCount < l.size() ? ComponentCount : l.size();
    size_t ctr = 0;
    for (const auto& item : l)
    {
        if (ctr == limit)
        {
            break;
        }

        mColors[ctr] = item;
        ctr++;
    }
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator=(const std::initializer_list<T>& l)
{
    memset(mColors, 0x0, sizeof(T) * ComponentCount);

    size_t limit = ComponentCount < l.size() ? ComponentCount : l.size();
    size_t ctr = 0;
    for (const auto& item : l)
    {
        if (ctr == limit)
        {
            break;
        }

        mColors[ctr] = item;
        ctr++;
    }
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>::Pixel(const Pixel<T, ComponentCount>& other)
{
    for (size_t i = 0; i < ComponentCount; ++i)
        mColors[i] = other.mColors[i];
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>::Pixel(Pixel<T, ComponentCount>&& other)
{
    for (size_t i = 0; i < ComponentCount; ++i)
        mColors[i] = other.mColors[i];
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator=(const Pixel<T, ComponentCount>& other)
{
    for (size_t i = 0; i < ComponentCount; ++i)
        mColors[i] = other.mColors[i];

    return *this;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator=(Pixel<T, ComponentCount>&& other)
{
    for (size_t i = 0; i < ComponentCount; ++i)
        mColors[i] = other.mColors[i];

    return *this;
}


template <typename T, size_t ComponentCount>
bool Pixel<T, ComponentCount>::operator==(const Pixel<T, ComponentCount>& other) const
{
    for (size_t i = 0; i < ComponentCount; ++i)
    {
        if (mColors[i] != other.mColors[i])
            return false;
    }

    return true;
}

template <typename T, size_t ComponentCount>
bool Pixel<T, ComponentCount>::operator!=(const Pixel<T, ComponentCount>& other) const
{
    for (size_t i = 0; i < ComponentCount; ++i)
    {
        if (mColors[i] != other.mColors[i])
            return true;
    }

    return false;
}

template <typename T, size_t ComponentCount>
bool Pixel<T, ComponentCount>::operator<(const Pixel<T, ComponentCount>& other) const
{
    for (size_t i = 0; i < ComponentCount; ++i)
    {
        if (mColors[i] >= other.mColors[i])
            return false;
    }

    return true;
}

template <typename T, size_t ComponentCount>
bool Pixel<T, ComponentCount>::operator>(const Pixel<T, ComponentCount>& other) const
{
    for (size_t i = 0; i < ComponentCount; ++i)
    {
        if (mColors[i] <= other.mColors[i])
            return false;
    }

    return true;
}

template <typename T, size_t ComponentCount>
bool Pixel<T, ComponentCount>::operator<=(const Pixel<T, ComponentCount>& other) const
{
    for (size_t i = 0; i < ComponentCount; ++i)
    {
        if (mColors[i] > other.mColors[i])
            return false;
    }

    return true;
}

template <typename T, size_t ComponentCount>
bool Pixel<T, ComponentCount>::operator>=(const Pixel<T, ComponentCount>& other) const
{
    for (size_t i = 0; i < ComponentCount; ++i)
    {
        if (mColors[i] < other.mColors[i])
            return false;
    }

    return true;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator+=(const Pixel<T, ComponentCount>& other)
{
    for (uint32_t i = 0; i < ComponentCount; ++i)
        mColors[i] += other.mColors[i];
    return *this;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator-=(const Pixel<T, ComponentCount>& other)
{
    for (uint32_t i = 0; i < ComponentCount; ++i)
        mColors[i] -= other.mColors[i];
    return *this;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator*=(const Pixel<T, ComponentCount>& other)
{
    for (uint32_t i = 0; i < ComponentCount; ++i)
        mColors[i] *= other.mColors[i];
    return *this;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator/=(const Pixel<T, ComponentCount>& other)
{
    for (uint32_t i = 0; i < ComponentCount; ++i)
        mColors[i] /= other.mColors[i];
    return *this;
}


template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator+=(const T& other)
{
    for (uint32_t i = 0; i < ComponentCount; ++i)
        mColors[i] += other;
    return *this;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator-=(const T& other)
{
    for (uint32_t i = 0; i < ComponentCount; ++i)
        mColors[i] -= other;
    return *this;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator*=(const T& other)
{
    for (uint32_t i = 0; i < ComponentCount; ++i)
        mColors[i] *= other;
    return *this;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator/=(const T& other)
{
    for (uint32_t i = 0; i < ComponentCount; ++i)
        mColors[i] /= other;
    return *this;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>& Pixel<T, ComponentCount>::operator^=(const T& exp)
{
    if (exp == static_cast<T>(0))
    {
        for (uint32_t i = 0; i < ComponentCount; ++i)
            mColors[i] = static_cast<T>(1);
        return *this;
    }

    for (uint32_t i = 0; i < ComponentCount; ++i)
        mColors[i] = static_cast<T>(pow(static_cast<float>(mColors[i]), static_cast<float>(exp)));
    return *this;
}

template <typename T, size_t ComponentCount>
T& Pixel<T, ComponentCount>::operator[](const size_t i)
{
    LKCOMMON_ASSERT(i < ComponentCount, "Too big index provided");
    return mColors[i];
}

template <typename T, size_t ComponentCount>
T Pixel<T, ComponentCount>::operator[](const size_t i) const
{
    LKCOMMON_ASSERT(i < ComponentCount, "Too big index provided");
    return mColors[i];
}

template <typename T, size_t ComponentCount>
template <typename ConvType>
Pixel<T, ComponentCount>::operator Pixel<ConvType, ComponentCount>() const
{
    Pixel<ConvType, ComponentCount> p;

    for (size_t i = 0; i < ComponentCount; ++i)
    {
        p.mColors[i] = ConvertColor(mColors[i]);
    }

    return p;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>::operator Pixel<float, 4>() const
{
    Pixel<float, 4> p;

    size_t limit = ComponentCount < 4 ? ComponentCount : 4;
    for (size_t i = 0; i < limit; ++i)
    {
        p.mColors.f[i] = ConvertColor(mColors[i]);
    }

    return p;
}

template <typename T, size_t ComponentCount>
void Pixel<T, ComponentCount>::Swap(size_t i, size_t j)
{
    T temp = mColors[i];
    mColors[i] = mColors[j];
    mColors[j] = temp;
}

template <size_t ComponentCount>
std::ostream& operator<< (std::ostream& o, const Pixel<uint8_t, ComponentCount>& p)
{
    o << "[";
    for (size_t i = 0; i < ComponentCount; ++i)
    {
        if (i != 0)
            o << ", ";
        o << static_cast<uint32_t>(p.mColors[i]);
    }
    o << "]";
    return o;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator+ (Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs)
{
    lhs += rhs;
    return lhs;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator- (Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs)
{
    lhs -= rhs;
    return lhs;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator* (Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs)
{
    lhs *= rhs;
    return lhs;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator/ (Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs)
{
    lhs /= rhs;
    return lhs;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator+ (Pixel<T, ComponentCount> lhs, const T& rhs)
{
    lhs += rhs;
    return lhs;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator- (Pixel<T, ComponentCount> lhs, const T& rhs)
{
    lhs -= rhs;
    return lhs;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator* (Pixel<T, ComponentCount> lhs, const T& rhs)
{
    lhs *= rhs;
    return lhs;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator/ (Pixel<T, ComponentCount> lhs, const T& rhs)
{
    lhs /= rhs;
    return lhs;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator^ (Pixel<T, ComponentCount> lhs, const T& exp)
{
    lhs ^= exp;
    return lhs;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> MinPixel(const Pixel<T, ComponentCount>& a, const Pixel<T, ComponentCount>& b)
{
    Pixel<T, ComponentCount> result;
    for (size_t i = 0; i < ComponentCount; ++i)
    {
        result.mColors[i] = a.mColors[i] < b.mColors[i] ? a.mColors[i] : b.mColors[i];
    }

    return result;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> MaxPixel(const Pixel<T, ComponentCount>& a, const Pixel<T, ComponentCount>& b)
{
    Pixel<T, ComponentCount> result;
    for (size_t i = 0; i < ComponentCount; ++i)
    {
        result.mColors[i] = a.mColors[i] > b.mColors[i] ? a.mColors[i] : b.mColors[i];
    }

    return result;
}


// 4 float component specialization

LKCOMMON_INLINE Pixel<float, 4>::Pixel()
    : mColors()
{
}

LKCOMMON_INLINE Pixel<float, 4>::Pixel(const float& color)
    : mColors(_mm_set_ps1(color))
{
}

LKCOMMON_INLINE Pixel<float, 4>::Pixel(const float colors[4])
    : mColors(_mm_load_ps(colors))
{
}

LKCOMMON_INLINE Pixel<float, 4>::Pixel(const float r, const float g, const float b, const float a)
    : mColors(r, g, b, a)
{
}

LKCOMMON_INLINE Pixel<float, 4>::Pixel(const __m128& m)
    : mColors(m)
{
}

LKCOMMON_INLINE Pixel<float, 4>::Pixel(const std::initializer_list<float>& l)
    : mColors()
{
    size_t limit = 4 < l.size() ? 4 : l.size();
    size_t ctr = 0;
    for (const auto& item : l)
    {
        if (ctr == limit)
        {
            break;
        }

        mColors.f[ctr] = item;
        ctr++;
    }
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator=(const std::initializer_list<float>& l)
{
    memset(&mColors.f, 0x0, sizeof(mColors));

    size_t limit = 4 < l.size() ? 4 : l.size();
    size_t ctr = 0;
    for (const auto& item : l)
    {
        if (ctr == limit)
        {
            break;
        }

        mColors.f[ctr] = item;
        ctr++;
    }

    return *this;
}

LKCOMMON_INLINE Pixel<float, 4>::Pixel(const Pixel<float, 4>& other)
    : mColors(other.mColors.m)
{
}

LKCOMMON_INLINE Pixel<float, 4>::Pixel(Pixel<float, 4>&& other)
    : mColors(other.mColors.m)
{
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator=(const Pixel<float, 4>& other)
{
    mColors.m = other.mColors.m;
    return *this;
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator=(Pixel<float, 4>&& other)
{
    mColors.m = other.mColors.m;
    return *this;
}


LKCOMMON_INLINE bool Pixel<float, 4>::operator==(const Pixel<float, 4>& other) const
{
    return (_mm_movemask_ps(_mm_cmpeq_ps(mColors.m, other.mColors.m)) == 0xF);
}

LKCOMMON_INLINE bool Pixel<float, 4>::operator!=(const Pixel<float, 4>& other) const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(mColors.m, other.mColors.m)) != 0);
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator+=(const Pixel<float, 4>& other)
{
    mColors.m = _mm_add_ps(mColors.m, other.mColors.m);
    return *this;
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator-=(const Pixel<float, 4>& other)
{
    mColors.m = _mm_sub_ps(mColors.m, other.mColors.m);
    return *this;
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator*=(const Pixel<float, 4>& other)
{
    mColors.m = _mm_mul_ps(mColors.m, other.mColors.m);
    return *this;
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator/=(const Pixel<float, 4>& other)
{
    mColors.m = _mm_div_ps(mColors.m, other.mColors.m);
    return *this;
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator+=(const float& other)
{
    mColors.m = _mm_add_ps(mColors.m, _mm_set_ps1(other));
    return *this;
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator-=(const float& other)
{
    mColors.m = _mm_sub_ps(mColors.m, _mm_set_ps1(other));
    return *this;
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator*=(const float& other)
{
    mColors.m = _mm_mul_ps(mColors.m, _mm_set_ps1(other));
    return *this;
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator/=(const float& other)
{
    mColors.m = _mm_div_ps(mColors.m, _mm_set_ps1(other));
    return *this;
}

LKCOMMON_INLINE Pixel<float, 4>& Pixel<float, 4>::operator^=(const float& exp)
{
    if (exp == 0.0f)
    {
        mColors.m = _mm_set_ps1(1.0f);
        return *this;
    }

    for (uint32_t i = 0; i < 4; ++i)
        mColors.f[i] = pow(mColors.f[i], exp);
    return *this;
}

LKCOMMON_INLINE float& Pixel<float, 4>::operator[](const size_t i)
{
    LKCOMMON_ASSERT(i < 4, "Too big index provided");
    return mColors.f[i];
}

LKCOMMON_INLINE float Pixel<float, 4>::operator[](const size_t i) const
{
    LKCOMMON_ASSERT(i < 4, "Too big index provided");
    return mColors.f[i];
}

template <typename ConvType>
LKCOMMON_INLINE Pixel<float, 4>::operator Pixel<ConvType, 4>() const
{
    Pixel<ConvType, 4> p;

    for (size_t i = 0; i < 4; ++i)
    {
        p.mColors[i] = ConvertColor(mColors.f[i]);
    }

    return p;
}

LKCOMMON_INLINE void Pixel<float, 4>::Swap(size_t i, size_t j)
{
    float temp = mColors.f[i];
    mColors.f[i] = mColors.f[j];
    mColors.f[j] = temp;
}

LKCOMMON_INLINE std::ostream& operator<< (std::ostream& o, const Pixel<float, 4>& p)
{
    o << "[" << p.mColors.f[0] << ", " << p.mColors.f[1] << ", " << p.mColors.f[2] << ", " << p.mColors.f[3] << "]";
    return o;
}

LKCOMMON_INLINE Pixel<float, 4> operator+ (const Pixel<float, 4>& lhs, const Pixel<float, 4>& rhs)
{
    return Pixel<float, 4>(_mm_add_ps(lhs.mColors.m, rhs.mColors.m));
}

LKCOMMON_INLINE Pixel<float, 4> operator- (const Pixel<float, 4>& lhs, const Pixel<float, 4>& rhs)
{
    return Pixel<float, 4>(_mm_sub_ps(lhs.mColors.m, rhs.mColors.m));
}

LKCOMMON_INLINE Pixel<float, 4> operator* (const Pixel<float, 4>& lhs, const Pixel<float, 4>& rhs)
{
    return Pixel<float, 4>(_mm_mul_ps(lhs.mColors.m, rhs.mColors.m));
}

LKCOMMON_INLINE Pixel<float, 4> operator/ (const Pixel<float, 4>& lhs, const Pixel<float, 4>& rhs)
{
    return Pixel<float, 4>(_mm_div_ps(lhs.mColors.m, rhs.mColors.m));
}

LKCOMMON_INLINE Pixel<float, 4> operator+ (const Pixel<float, 4>& lhs, const float& rhs)
{
    return Pixel<float, 4>(_mm_add_ps(lhs.mColors.m, _mm_set_ps1(rhs)));
}

LKCOMMON_INLINE Pixel<float, 4> operator- (const Pixel<float, 4>& lhs, const float& rhs)
{
    return Pixel<float, 4>(_mm_sub_ps(lhs.mColors.m, _mm_set_ps1(rhs)));
}

LKCOMMON_INLINE Pixel<float, 4> operator* (const Pixel<float, 4>& lhs, const float& rhs)
{
    return Pixel<float, 4>(_mm_mul_ps(lhs.mColors.m, _mm_set_ps1(rhs)));
}

LKCOMMON_INLINE Pixel<float, 4> operator/ (const Pixel<float, 4>& lhs, const float& rhs)
{
    return Pixel<float, 4>(_mm_div_ps(lhs.mColors.m, _mm_set_ps1(rhs)));
}

LKCOMMON_INLINE Pixel<float, 4> operator^ (const Pixel<float, 4>& lhs, const float& exp)
{
    if (exp == 0)
        return Pixel<float, 4>(1.0f);

    Pixel<float, 4> result(lhs);
    for (uint32_t i = 0; i < 4; ++i)
        result.mColors.f[i] = pow(lhs.mColors.f[i], exp);

    return result;
}

LKCOMMON_INLINE Pixel<float, 4> MinPixel(const Pixel<float, 4>& a, const Pixel<float, 4>& b)
{
    return Pixel<float, 4>(_mm_min_ps(a.mColors.m, b.mColors.m));
}

LKCOMMON_INLINE Pixel<float, 4> MaxPixel(const Pixel<float, 4>& a, const Pixel<float, 4>& b)
{
    return Pixel<float, 4>(_mm_max_ps(a.mColors.m, b.mColors.m));
}

} // namespace Utils
} // namespace lkCommon
