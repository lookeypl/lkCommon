/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Pixel utility template class implementation.
 */

#pragma once

#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <xmmintrin.h>
#include <lkCommon/lkCommon.hpp>


namespace lkCommon {
namespace Utils {

// predeclarations for Pixel class definition, needed for ostream operator overload
template <typename T, size_t ComponentCount> struct Pixel;

/**
 * Operator declarations for friendships
 */
template <typename T, size_t ComponentCount>
std::ostream& operator<< (std::ostream& o, const Pixel<T, ComponentCount>& p);
template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator+ (Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs);
template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator- (Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs);
template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator* (Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs);
template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator/ (Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs);
template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator+ (Pixel<T, ComponentCount> lhs, const T& rhs);
template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator- (Pixel<T, ComponentCount> lhs, const T& rhs);
template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator* (Pixel<T, ComponentCount> lhs, const T& rhs);
template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator/ (Pixel<T, ComponentCount> lhs, const T& rhs);
template <typename T, size_t ComponentCount>
Pixel<T, ComponentCount> operator^ (Pixel<T, ComponentCount> lhs, const T& exp);

/**
 * Declaration of operator specializations for 4-component float specialization
 */
std::ostream& operator<< (std::ostream& o, const Pixel<float, 4>& p);
Pixel<float, 4> operator+ (const Pixel<float, 4>& lhs, const Pixel<float, 4>& rhs);
Pixel<float, 4> operator- (const Pixel<float, 4>& lhs, const Pixel<float, 4>& rhs);
Pixel<float, 4> operator* (const Pixel<float, 4>& lhs, const Pixel<float, 4>& rhs);
Pixel<float, 4> operator/ (const Pixel<float, 4>& lhs, const Pixel<float, 4>& rhs);
Pixel<float, 4> operator+ (const Pixel<float, 4>& lhs, const float& rhs);
Pixel<float, 4> operator- (const Pixel<float, 4>& lhs, const float& rhs);
Pixel<float, 4> operator* (const Pixel<float, 4>& lhs, const float& rhs);
Pixel<float, 4> operator/ (const Pixel<float, 4>& lhs, const float& rhs);
Pixel<float, 4> operator^ (const Pixel<float, 4>& lhs, const float& exp);


/**
 * Template structure containing a single Pixel of given type @p T with
 * @p ComponentCount components.
 *
 * Its main goal is to provide a common
 */
template <typename T, size_t ComponentCount>
struct Pixel
{
    // some assertions to limit uses for our class
    static_assert(ComponentCount > 0, "Cannot create a Pixel with 0 components");
    static_assert(std::is_same<uint8_t, typename std::remove_cv<T>::type>::value ||
                  std::is_same<float, typename std::remove_cv<T>::type>::value,
                  "Unsupported template type. Only supported types are: uint8_t, float");

    // container for colors
    T mColors[ComponentCount];

    // Constructors and assignment from initializer list
    Pixel();
    Pixel(const T& color);
    Pixel(const T colors[ComponentCount]);
    Pixel(const std::initializer_list<T>& l);
    Pixel& operator=(const std::initializer_list<T>& l);

    // Copy/move constructors
    Pixel(const Pixel& other);
    Pixel(Pixel&& other);
    Pixel& operator=(const Pixel& other);
    Pixel& operator=(Pixel&& other);

    // comparison operators
    bool operator==(const Pixel<T, ComponentCount>& other) const;
    bool operator!=(const Pixel<T, ComponentCount>& other) const;

    // arithmetic operators vs other Pixel
    // NOTE these do *not* check for overflows
    Pixel<T, ComponentCount>& operator+=(const Pixel<T, ComponentCount>& other);
    Pixel<T, ComponentCount>& operator-=(const Pixel<T, ComponentCount>& other);
    Pixel<T, ComponentCount>& operator*=(const Pixel<T, ComponentCount>& other);
    Pixel<T, ComponentCount>& operator/=(const Pixel<T, ComponentCount>& other);

    // arithmetic operators vs a single component type
    // NOTE these do *not* check for overflows
    Pixel<T, ComponentCount>& operator+=(const T& other);
    Pixel<T, ComponentCount>& operator-=(const T& other);
    Pixel<T, ComponentCount>& operator*=(const T& other);
    Pixel<T, ComponentCount>& operator/=(const T& other);
    Pixel<T, ComponentCount>& operator^=(const T& exp);

    // array subscript operator for easy access to components
    T operator[](size_t i) const;

    // cast operator
    // NOTE casting from higher precision type to lower (ex. float -> uint8_t) MIGHT clamp the result
    template <typename ConvType>
    operator Pixel<ConvType, ComponentCount>() const;

    // cast to 4-component float specialization, separate because Pixel<float, 4> uses different data container
    operator Pixel<float, 4>() const;

    // internally swaps components together
    // NOTE do NOT confuse with std::swap
    void Swap(size_t i, size_t j);

    // friendships
    friend std::ostream& operator<< <T, ComponentCount>(std::ostream& o, const Pixel<T, ComponentCount>& p);
    friend Pixel<T, ComponentCount> operator+ <T, ComponentCount>(Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs);
    friend Pixel<T, ComponentCount> operator- <T, ComponentCount>(Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs);
    friend Pixel<T, ComponentCount> operator* <T, ComponentCount>(Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs);
    friend Pixel<T, ComponentCount> operator/ <T, ComponentCount>(Pixel<T, ComponentCount> lhs, const Pixel<T, ComponentCount>& rhs);
    friend Pixel<T, ComponentCount> operator+ <T, ComponentCount>(Pixel<T, ComponentCount> lhs, const T& rhs);
    friend Pixel<T, ComponentCount> operator- <T, ComponentCount>(Pixel<T, ComponentCount> lhs, const T& rhs);
    friend Pixel<T, ComponentCount> operator* <T, ComponentCount>(Pixel<T, ComponentCount> lhs, const T& rhs);
    friend Pixel<T, ComponentCount> operator/ <T, ComponentCount>(Pixel<T, ComponentCount> lhs, const T& rhs);
    friend Pixel<T, ComponentCount> operator^ <T, ComponentCount>(Pixel<T, ComponentCount> lhs, const T& exp);
};

/**
 * Pixel template specialization for 4 float components, implemented using SSE for speedup.
 */
template <>
struct Pixel<float, 4>
{
    // container for colors
    union Pixel4f
    {
        float f[4];
        __m128 m;

        LKCOMMON_INLINE Pixel4f()
            : m(_mm_setzero_ps())
        {}

        LKCOMMON_INLINE Pixel4f(const __m128& m)
            : m(m)
        {}

        LKCOMMON_INLINE Pixel4f(const float f0, const float f1, const float f2, const float f3)
            : f{f0, f1, f2, f3}
        {}
    } mColors;

    // Constructors and assignment from initializer list
    Pixel();
    Pixel(const float& color);
    Pixel(const float colors[4]);
    Pixel(const float r, const float g, const float b, const float a);
    Pixel(const __m128& m);
    Pixel(const std::initializer_list<float>& l);
    Pixel& operator=(const std::initializer_list<float>& l);

    // Copy/move constructors
    Pixel(const Pixel<float, 4>& other);
    Pixel(Pixel<float, 4>&& other);
    Pixel& operator=(const Pixel<float, 4>& other);
    Pixel& operator=(Pixel<float, 4>&& other);

    // comparison operators
    bool operator==(const Pixel<float, 4>& other) const;
    bool operator!=(const Pixel<float, 4>& other) const;

    // arithmetic operators vs other Pixel
    // NOTE these do *not* check for overflows
    Pixel<float, 4>& operator+=(const Pixel<float, 4>& other);
    Pixel<float, 4>& operator-=(const Pixel<float, 4>& other);
    Pixel<float, 4>& operator*=(const Pixel<float, 4>& other);
    Pixel<float, 4>& operator/=(const Pixel<float, 4>& other);

    // arithmetic operators vs a single component type
    // NOTE these do *not* check for overflows
    Pixel<float, 4>& operator+=(const float& other);
    Pixel<float, 4>& operator-=(const float& other);
    Pixel<float, 4>& operator*=(const float& other);
    Pixel<float, 4>& operator/=(const float& other);
    Pixel<float, 4>& operator^=(const float& exp);

    // array subscript operator for easy access to components
    float operator[](size_t i) const;

    // cast operator
    // NOTE casting from higher precision type to lower (ex. float -> uint8_t) MIGHT clamp the result
    template <typename ConvType>
    operator Pixel<ConvType, 4>() const;

    // internally swaps components together
    // NOTE do NOT confuse with std::swap
    void Swap(size_t i, size_t j);

    // friendships
    friend std::ostream& operator<< <float, 4>(std::ostream& o, const Pixel<float, 4>& p);
    friend Pixel<float, 4> operator+ <float, 4>(Pixel<float, 4> lhs, const Pixel<float, 4>& rhs);
    friend Pixel<float, 4> operator- <float, 4>(Pixel<float, 4> lhs, const Pixel<float, 4>& rhs);
    friend Pixel<float, 4> operator* <float, 4>(Pixel<float, 4> lhs, const Pixel<float, 4>& rhs);
    friend Pixel<float, 4> operator/ <float, 4>(Pixel<float, 4> lhs, const Pixel<float, 4>& rhs);
    friend Pixel<float, 4> operator+ <float, 4>(Pixel<float, 4> lhs, const float& rhs);
    friend Pixel<float, 4> operator- <float, 4>(Pixel<float, 4> lhs, const float& rhs);
    friend Pixel<float, 4> operator* <float, 4>(Pixel<float, 4> lhs, const float& rhs);
    friend Pixel<float, 4> operator/ <float, 4>(Pixel<float, 4> lhs, const float& rhs);
    friend Pixel<float, 4> operator^ <float, 4>(Pixel<float, 4> lhs, const float& exp);
};

using PixelFloat4 = lkCommon::Utils::Pixel<float, 4>;
using PixelUint4 = lkCommon::Utils::Pixel<uint8_t, 4>;

} // namespace Utils
} // namespace lkCommon

#include "PixelImpl.hpp"
