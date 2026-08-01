#include "lkCommon/Math/Vector2.hpp"

#include "lkCommon/Math/Vector3.hpp"
#include "lkCommon/Math/Vector4.hpp"
#include <cmath>


namespace lkCommon {
namespace Math {

// Casts
Vector2::operator Vector3() const
{
    return Vector3(
        mValue[0],
        mValue[1],
        0.0f
    );
}

Vector2::operator Vector4() const
{
    return Vector4(
        mValue[0],
        mValue[1],
        0.0f,
        0.0f
    );
}

// Friendships
std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
    os << "[" << v.mValue[0] << ", " << v.mValue[1] << "]";
    return os;
}

} // namespace Math
} // namespace lkCommon
