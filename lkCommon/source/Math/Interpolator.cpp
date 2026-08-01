#include "lkCommon/Math/Interpolator.hpp"


namespace lkCommon {
namespace Math {

Interpolator::Interpolator()
{
}

void Interpolator::Add(const Vector4& point)
{
    mPoints.emplace_back(point);
}

void Interpolator::Add(const InterpolatorPoints& points)
{
    mPoints.insert(mPoints.end(), points.begin(), points.end());
}

void Interpolator::Clear()
{
    mPoints.clear();
}

Vector4 Interpolator::Interpolate(float factor)
{
    if (mPoints.size() == 0)
        return Vector4(0.0f);
    if (mPoints.size() == 1)
        return mPoints[0];

    mOutOfRange = false;
    if (factor < 0.0f)
    {
        mOutOfRange = true;
        return mPoints[0];
    }
    else if (factor >= mPoints.size() - 1)
    {
        mOutOfRange = true;
        return mPoints.back();
    }

    // determine where we are in our point list
    uint32_t point0 = static_cast<uint32_t>(factor);
    factor -= point0;

    // call backend
    return InterpolatePoints(mPoints[point0], mPoints[point0 + 1], factor);
}

} // namespace Math
} // namespace lkCommon
