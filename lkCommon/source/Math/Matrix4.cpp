#include "lkCommon/Math/Matrix4.hpp"


namespace lkCommon {
namespace Math {

// Constant definitions
const Matrix4 Matrix4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f);


// Friendships
std::ostream& operator<<(std::ostream& os, const Matrix4& m)
{
    os << "[" << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << "]";
    return os;
}

// Matrix4 creators
Matrix4 Matrix4::CreateRHLookAt(const Vector4& pos, const Vector4& dir, const Vector4& up)
{
    const Vector4 zAxis = pos - dir;
    zAxis.Normalize();

    const Vector4 xAxis = up.Cross(zAxis);
    xAxis.Normalize();

    const Vector4 yAxis = zAxis.Cross(xAxis);
    // No normalization needed here, since we cross two already normalized Vector4s.

    return Matrix4(         xAxis[0],          xAxis[1],          xAxis[2], -(xAxis.Dot(pos)),
                            yAxis[0],          yAxis[1],          yAxis[2], -(yAxis.Dot(pos)),
                            zAxis[0],          zAxis[1],          zAxis[2], -(zAxis.Dot(pos)),
                                0.0f,              0.0f,              0.0f,              1.0f);
}

Matrix4 Matrix4::CreateRHProjection(const float fov, const float aspectRatio, const float nearDist, const float farDist)
{
    const float halfFov = fov * 0.5f;
    const float yScale = 1.0f / tanf(halfFov); // aka. ctg(halfFov)
    const float xScale = yScale / aspectRatio;

    const float distDiff = nearDist - farDist;
    const float zVal1 = farDist / distDiff;
    const float zVal2 = (nearDist * farDist) / distDiff;

    return Matrix4(xScale,   0.0f,  0.0f,  0.0f,
                     0.0f, yScale,  0.0f,  0.0f,
                     0.0f,   0.0f, zVal1, zVal2,
                     0.0f,   0.0f, -1.0f,  0.0f);
}

Matrix4 Matrix4::CreateRotationX(const float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f,    c,   -s, 0.0f,
                   0.0f,    s,    c, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4 Matrix4::CreateRotationY(const float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return Matrix4(   c, 0.0f,    s, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                     -s, 0.0f,    c, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4 Matrix4::CreateRotationZ(const float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return Matrix4(   c,   -s, 0.0f, 0.0f,
                      s,    c, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4 Matrix4::CreateTranslation(const Vector4& translation)
{
    Matrix4 result(Matrix4::IDENTITY);

    result[3][0] = translation[0];
    result[3][1] = translation[1];
    result[3][2] = translation[2];

    return result;
}

Matrix4 Matrix4::CreateScale(const float scale)
{
    Matrix4 result(Matrix4::IDENTITY);

    result[0][0] = scale;
    result[1][1] = scale;
    result[2][2] = scale;

    return result;
}

Matrix4 Matrix4::CreateScale(const float scaleX, const float scaleY, const float scaleZ)
{
    Matrix4 result(Matrix4::IDENTITY);

    result[0][0] = scaleX;
    result[1][1] = scaleY;
    result[2][2] = scaleZ;

    return result;
}

Matrix4 Matrix4::CreateScale(const Vector4& scale)
{
    Matrix4 result(Matrix4::IDENTITY);

    result[0][0] = scale[0];
    result[1][1] = scale[1];
    result[2][2] = scale[2];

    return result;
}

} // namespace Math
} // namespace lkCommon
