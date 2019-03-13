#pragma once

#ifndef _LKCOMMON_MATH_UTILITIES_HPP_
#error "Please include main header of Utilities, not the implementation header."
#endif // _LKCOMMON_MATH_UTILITIES_HPP_


namespace lkCommon {
namespace Math {
namespace Util {

template <typename T>
T Lerp(const T& a, const T& b, const float factor)
{
    return a * (1.0f - factor) + b * factor;
}

}
}
}
