#include "lkCommon/Math/Vector4.hpp"

#include <smmintrin.h>
#include <cmath>


namespace lkCommon {
namespace Math {

// Friendships
std::ostream& operator<<(std::ostream& os, const Vector4& v)
{
    os << "[" << v.mValue.f[0] << ", " << v.mValue.f[1] << ", " << v.mValue.f[2] << ", " << v.mValue.f[3] << "]";
    return os;
}

} // namespace Math
} // namespace lkCommon
