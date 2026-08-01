#include <gtest/gtest.h>

#include <lkCommon/Math/Vector3.hpp>

#include <lkCommon/Math/Vector2.hpp>
#include <lkCommon/Math/Vector4.hpp>

using namespace lkCommon::Math;


namespace {

const Vector3 resultZero(0.0f, 0.0f, 0.0f);
const Vector3 resultOne(1.0f, 1.0f, 1.0f);
const Vector3 resultAdd(3.0f, 5.0f, 7.0f);
const Vector3 resultAdd2(3.0f, 4.0f, 5.0f);
const Vector3 resultSub(-1.0f, -1.0f, -1.0f);
const Vector3 resultSub2(-1.0f, 0.0f, 1.0f);
const Vector3 resultMul(2.0f, 6.0f, 12.0f);
const Vector3 resultMul2(2.0f, 4.0f, 6.0f);
const Vector3 resultDiv(0.5f, 0.5f, 0.5f);
const Vector3 resultDiv2(0.5f, 1.0f, 1.5f);
const Vector3 resultCross(0.0f, 0.0f, 1.0f);
const Vector3 resultNorm(1.0f, 0.0f, 0.0f);
const Vector3 vectorToCast(1.0f, 2.0f, 3.0f);
const Vector2 resultCastV2(1.0f, 2.0f);
const Vector4 resultCastV4(1.0f, 2.0f, 3.0f, 0.0f);

} // namespace

TEST(Vector3, Constructor)
{
    Vector3 a;
    ASSERT_EQ(resultZero, a);
}

TEST(Vector3, ConstructorSingleFloat)
{
    Vector3 a(1.0f);
    ASSERT_EQ(resultOne, a);
}

TEST(Vector3, CopyConstructor)
{
    Vector3 v(resultOne);
    ASSERT_EQ(v, resultOne);
}

TEST(Vector3, MoveConstructor)
{
    Vector3 v(resultOne);
    Vector3 v2(std::move(v));
    ASSERT_EQ(v2, resultOne);
}

TEST(Vector3, CopySubstitution)
{
    Vector3 v(2.0f);
    ASSERT_NE(v, resultOne);
    v = resultOne;
    ASSERT_EQ(v, resultOne);
}

TEST(Vector3, MoveSubstitution)
{
    Vector3 v(resultOne);
    Vector3 v2(2.0f);
    ASSERT_NE(v, v2);
    v2 = std::move(v);
    ASSERT_EQ(v2, resultOne);
}

TEST(Vector3, AddVector3)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(2.0f, 3.0f, 4.0f);

    ASSERT_EQ(resultAdd, (a + b));
}

TEST(Vector3, AddFloat)
{
    Vector3 a(1.0f, 2.0f, 3.0f);

    ASSERT_EQ(resultAdd2, (a + 2.0f));
}

TEST(Vector3, SubtractVector3)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(2.0f, 3.0f, 4.0f);

    ASSERT_EQ(resultSub, (a - b));
}

TEST(Vector3, SubtractFloat)
{
    Vector3 a(1.0f, 2.0f, 3.0f);

    ASSERT_EQ(resultSub2, (a - 2.0f));
}

TEST(Vector3, MultiplyVector3)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(2.0f, 3.0f, 4.0f);

    ASSERT_EQ(resultMul, (a * b));
}

TEST(Vector3, MultiplyFloat)
{
    Vector3 a(1.0f, 2.0f, 3.0f);

    ASSERT_EQ(resultMul2, (a * 2.0f));
}

TEST(Vector3, DivideVector3)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(2.0f, 4.0f, 6.0f);

    ASSERT_EQ(resultDiv, (a / b));
}

TEST(Vector3, DivideFloat)
{
    Vector3 a(1.0f, 2.0f, 3.0f);

    ASSERT_EQ(resultDiv2, (a / 2.0f));
}

TEST(Vector3, Dot)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(2.0f, 3.0f, 4.0f);

    ASSERT_EQ(20.0f, a.Dot(b));
}

TEST(Vector3, Cross)
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(0.0f, 1.0f, 0.0f);

    ASSERT_EQ(resultCross, a.Cross(b));
}

TEST(Vector3, Length)
{
    Vector3 a(2.0f, 0.0f, 0.0f);

    ASSERT_EQ(2.0f, a.Length());
}

TEST(Vector3, Normalize)
{
    Vector3 a(2.0f, 0.0f, 0.0f);
    ASSERT_EQ(resultNorm, a.Normalize());
}

TEST(Vector3, Equal)
{
    Vector3 a(1.0f);
    Vector3 b(1.0f);
    Vector3 c(1.0f, 1.0f, 1.1f);

    ASSERT_TRUE(a == b);
    ASSERT_FALSE(a == c);
}

TEST(Vector3, NotEqual)
{
    Vector3 a(1.0f);
    Vector3 b(1.0f, 1.0f, 1.1f);
    Vector3 c(1.0f);

    ASSERT_TRUE(a != b);
    ASSERT_FALSE(a != c);
}

TEST(Vector3, CastToVector2)
{
    Vector3 v1(vectorToCast);
    Vector2 v2 = static_cast<Vector2>(v1);

    ASSERT_EQ(v2, resultCastV2);
}

TEST(Vector3, CastToVector4)
{
    Vector3 v1(vectorToCast);
    Vector4 v2 = static_cast<Vector4>(v1);

    ASSERT_EQ(v2, resultCastV4);
}
