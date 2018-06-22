#pragma once

#include "../Vector4.hpp"

#include <vector>


namespace lkCommon {
namespace Math {

using InterpolatorPoints = std::vector<Vector4>;

class Interpolator
{
protected:
    InterpolatorPoints mPoints;
    bool mOutOfRange;

    virtual Vector4 InterpolatePoints(const Vector4& p1, const Vector4& p2, float factor) = 0;

public:
    Interpolator();

    void Add(const Vector4& point);
    void Add(const InterpolatorPoints& points);
    void Clear();
    Vector4 Interpolate(float factor);

    bool OutOfRange() const
    {
        return mOutOfRange;
    }
};

} // namespace Math
} // namespace lkCommon
