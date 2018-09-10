#include "Pixel.hpp"
#pragma once


namespace {

// converters

template <typename Src, typename Dst>
LKCOMMON_INLINE Dst ConvertColor(Src source)
{
    static_assert(std::is_same<uint8_t, typename std::remove_cv<Src>::type>::value
               || std::is_same<float, typename std::remove_cv<Src>::type>::value,
               "Incompatible types provided for conversion");
}

template <>
LKCOMMON_INLINE uint8_t ConvertColor(float source)
{
    return static_cast<uint8_t>(source * 255);
}

template <>
LKCOMMON_INLINE float ConvertColor(uint8_t source)
{
    return static_cast<float>(source) / 255.0f;
}


// value range clamps per type (ex. limits float to 0.0f - 1.0f range)
template <typename T>
LKCOMMON_INLINE T Clamp(T x)
{
    staic_assert(std::is_same<uint8_t, typename std::remove_cv<T>::type>::value
              || std::is_same<float, typename std::remove_cv<T>::type>::value,
              "Incompatible type provided for clamping");
}

template <>
LKCOMMON_INLINE uint8_t Clamp(uint8_t x)
{
    return x;
}

template <>
LKCOMMON_INLINE float Clamp(float x)
{
    x = (x > 1.0f ? 1.0f : x);
    return (x < 0.0f ? 0.0f : x);
}


} // namespace


namespace lkCommon {
namespace Utils {

// generic constructors

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>::Pixel()
    : mColors{static_cast<T>(0)}
{
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>::Pixel(T color)
{
    color = Clamp(color);
    for (size_t i = 0; i < ComponentCount; ++i)
        mColors[i] = color;
}

template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount>::Pixel(const T colors[ComponentCount])
{
    for (size_t i = 0; i < ComponentCount; ++i)
        mColors[i] = Clamp(colors[i]);
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
template <typename ConvType>
Pixel<T, ComponentCount>::operator Pixel<ConvType, ComponentCount>() const
{
    Pixel<ConvType, ComponentCount> p;

    for (size_t i = 0; i < ComponentCount; ++i)
    {
        p.mColors[i] = ConvertColor<T, ConvType>(mColors[i]);
    }

    return p;
}

template <typename T, size_t ComponentCount>
std::ostream& operator<<(std::ostream& o, const Pixel<T, ComponentCount>& p)
{
    o << "[";
    for (size_t i = 0; i < ComponentCount; ++i)
    {
        if (i != 0)
            o << ", ";
        o << p.mColors[i];
    }
    o << "]";
    return o;
}

} // namespace Utils
} // namespace lkCommon
