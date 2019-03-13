#include "lkCommon/Math/LinearInterpolator.hpp"

#include "lkCommon/Math/Utilities.hpp"


namespace lkCommon {
namespace Math {

Vector4 LinearInterpolator::InterpolatePoints(const Vector4& p1, const Vector4& p2, float factor)
{
    return Util::Lerp(p1, p2, factor);
}

} // namespace Math
} // namespace lkCommon
