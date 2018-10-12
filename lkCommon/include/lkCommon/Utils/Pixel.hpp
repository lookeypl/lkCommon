/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Pixel utility template class implementation.
 */

#pragma once

#include <cstdint>
#include <type_traits>
#include <initializer_list>
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

    // container for the colors
    T mColors[ComponentCount];

    // Constructors and assignment from initializer list
    Pixel();
    Pixel(T color);
    Pixel(const T colors[ComponentCount]);
    Pixel(std::initializer_list<T> l);
    Pixel& operator=(std::initializer_list<T> l);

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

    // array subscript operator for easy access to components
    T operator[](size_t i) const;

    // cast operator
    // NOTE casting from higher precision type to lower (ex. float -> uint8_t) MIGHT clamp the result
    template <typename ConvType>
    operator Pixel<ConvType, ComponentCount>() const;

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
};

using PixelFloat4 = lkCommon::Utils::Pixel<float, 4>;
using PixelUint4 = lkCommon::Utils::Pixel<uint8_t, 4>;

} // namespace Utils
} // namespace lkCommon

#include "PixelImpl.hpp"
