#include "lkCommon/Math/Vector4.hpp"

#include "lkCommon/Math/Vector2.hpp"
#include "lkCommon/Math/Vector3.hpp"
#include <smmintrin.h>
#include <cmath>


namespace lkCommon {
namespace Math {

// Casts
Vector4::operator Vector2() const
{
    return Vector2(
        mValue.f[0],
        mValue.f[1]
    );
}

Vector4::operator Vector3() const
{
    return Vector3(
        mValue.f[0],
        mValue.f[1],
        mValue.f[2]
    );
}

// Friendships
std::ostream& operator<<(std::ostream& os, const Vector4& v)
{
    os << "[" << v.mValue.f[0] << ", " << v.mValue.f[1] << ", " << v.mValue.f[2] << ", " << v.mValue.f[3] << "]";
    return os;
}

} // namespace Math
} // namespace lkCommon
