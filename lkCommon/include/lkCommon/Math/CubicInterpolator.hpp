#pragma once

#include "lkCommon/Math/Interpolator.hpp"


namespace lkCommon {
namespace Math {

class CubicInterpolator: public Interpolator
{
    Vector4 InterpolatePoints(const Vector4& p1, const Vector4& p2, float factor) override;
};

} // namespace Math
} // namespace lkCommon
