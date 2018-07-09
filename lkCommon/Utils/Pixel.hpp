#pragma once

#include <cstdint>
#include <type_traits>
#include <lkCommon/lkCommon.hpp>

namespace lkCommon {
namespace Utils {

// predeclarations for Pixel class definition
template <typename T, size_t ComponentCount> class Pixel;

template <typename T, size_t ComponentCount>
std::ostream& operator<< (std::ostream& o, const Pixel<T, ComponentCount>& p);


template <typename T, size_t ComponentCount>
class Pixel
{
    static_assert(std::is_same<uint8_t, typename std::remove_cv<T>::type>::value
               || std::is_same<float, typename std::remove_cv<T>::type>::value,
                  "Unsupported template type. Only provided types are: uint8_t, float");

public:
    T mColors[ComponentCount];

    Pixel();
    Pixel(T color);
    Pixel(const T colors[ComponentCount]);

    LKCOMMON_INLINE T* GetColors();

    bool operator==(const Pixel<T, ComponentCount>& other) const;
    bool operator!=(const Pixel<T, ComponentCount>& other) const;

    template <typename ConvType>
    operator Pixel<ConvType, ComponentCount>() const;

    friend std::ostream& operator<< <T, ComponentCount>(std::ostream& o, const Pixel<T, ComponentCount>& p);
};

} // namespace Utils
} // namespace lkCommon

#include "PixelImpl.hpp"
