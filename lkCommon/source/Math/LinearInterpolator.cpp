#include "lkCommon/Math/LinearInterpolator.hpp"


namespace lkCommon {
namespace Math {

Vector4 LinearInterpolator::InterpolatePoints(const Vector4& p1, const Vector4& p2, float factor)
{
    return p1 * (1.0f - factor) + p2 * factor;
}

} // namespace Math
} // namespace lkCommon
