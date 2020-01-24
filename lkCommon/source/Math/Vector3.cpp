#include "lkCommon/Math/Vector3.hpp"

#include "lkCommon/Math/Vector2.hpp"
#include "lkCommon/Math/Vector4.hpp"
#include <cmath>


namespace lkCommon {
namespace Math {

// Casts
Vector3::operator Vector2() const
{
    return Vector2(
        mValue[0],
        mValue[1]
    );
}

Vector3::operator Vector4() const
{
    return Vector4(
        mValue[0],
        mValue[1],
        mValue[2],
        0.0f
    );
}

// Friendships
std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
    os << "[" << v.mValue[0] << ", " << v.mValue[1] << ", " << v.mValue[2] << "]";
    return os;
}

} // namespace Math
} // namespace lkCommon
