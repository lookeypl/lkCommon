#include "lkCommon/Math/Utilities.hpp"
#include "lkCommon/Math/Random.hpp"
#include "lkCommon/Math/Constants.hpp"


namespace lkCommon {
namespace Math {
namespace Util {

Vector4 RotateFromToVector(const Vector4& v, const Vector4& from, const Vector4& to)
{
    // form rotation info based on from-to vectors
    const Vector4 rotAxis = from.Cross(to);
    const float cosAngle = from.Dot(to);
    const float angle = acosf(cosAngle);
    const float sinAngle = sinf(angle);

    // use Rodrigues' rotation formula to rotate v to new position
    return (v * cosAngle) + (rotAxis.Cross(v) * sinAngle) + (rotAxis * (rotAxis.Dot(v)) * (1.0f - cosAngle));
}

Vector4 CosineSampleHemisphere(const Vector4& normal, const float x1, const float x2)
{
    // use x1 and x2 to create a random point from a disk
    const float r = sqrtf(x1);
    const float theta = 2 * LKCOMMON_PIF * x2;

    const float x = r * cosf(theta);
    const float y = r * sinf(theta);

    // project points onto tangent-space hemisphere
    const float z = 1.0f - x1;
    Vector4 result(x, y, sqrtf(z > 0.0f ? z : 0.0f), 0.0f);

    // rotate resulting vector from Z to normal
    return RotateFromToVector(result, Vector4(0.0f, 0.0f, 1.0f, 0.0f), normal);
}

Vector4 CosineSampleHemisphere(const Vector4& normal)
{
    return CosineSampleHemisphere(normal, Random::Xorshift(), Random::Xorshift());
}

} // namespace Util
} // namespace Math
} // namespace lkCommon
