#include <gtest/gtest.h>

#include <lkCommon/Math/Vector2.hpp>

#include <lkCommon/Math/Vector3.hpp>
#include <lkCommon/Math/Vector4.hpp>

using namespace lkCommon::Math;


namespace {

const Vector2 resultZero(0.0f, 0.0f);
const Vector2 resultOne(1.0f, 1.0f);
const Vector2 resultAdd(3.0f, 5.0f);
const Vector2 resultAdd2(3.0f, 4.0f);
const Vector2 resultSub(-1.0f, -1.0f);
const Vector2 resultSub2(-1.0f, 0.0f);
const Vector2 resultMul(2.0f, 6.0f);
const Vector2 resultMul2(2.0f, 4.0f);
const Vector2 resultDiv(0.5f, 0.5f);
const Vector2 resultDiv2(0.5f, 1.0f);
const Vector2 resultNorm(1.0f, 0.0f);
const Vector2 vectorToCast(1.0f, 2.0f);
const Vector3 resultCastV3(1.0f, 2.0f, 0.0f);
const Vector4 resultCastV4(1.0f, 2.0f, 0.0f, 0.0f);

} // namespace

TEST(Vector2, Constructor)
{
    Vector2 a;
    ASSERT_EQ(resultZero, a);
}

TEST(Vector2, ConstructorSingleFloat)
{
    Vector2 a(1.0f);
    ASSERT_EQ(resultOne, a);
}

TEST(Vector2, CopyConstructor)
{
    Vector2 v(resultOne);
    ASSERT_EQ(v, resultOne);
}

TEST(Vector2, MoveConstructor)
{
    Vector2 v(resultOne);
    Vector2 v2(std::move(v));
    ASSERT_EQ(v2, resultOne);
}

TEST(Vector2, CopySubstitution)
{
    Vector2 v(2.0f);
    ASSERT_NE(v, resultOne);
    v = resultOne;
    ASSERT_EQ(v, resultOne);
}

TEST(Vector2, MoveSubstitution)
{
    Vector2 v(resultOne);
    Vector2 v2(2.0f);
    ASSERT_NE(v, v2);
    v2 = std::move(v);
    ASSERT_EQ(v2, resultOne);
}

TEST(Vector2, AddVector2)
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(2.0f, 3.0f);

    ASSERT_EQ(resultAdd, (a + b));
}

TEST(Vector2, AddFloat)
{
    Vector2 a(1.0f, 2.0f);

    ASSERT_EQ(resultAdd2, (a + 2.0f));
}

TEST(Vector2, SubtractVector2)
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(2.0f, 3.0f);

    ASSERT_EQ(resultSub, (a - b));
}

TEST(Vector2, SubtractFloat)
{
    Vector2 a(1.0f, 2.0f);

    ASSERT_EQ(resultSub2, (a - 2.0f));
}

TEST(Vector2, MultiplyVector2)
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(2.0f, 3.0f);

    ASSERT_EQ(resultMul, (a * b));
}

TEST(Vector2, MultiplyFloat)
{
    Vector2 a(1.0f, 2.0f);

    ASSERT_EQ(resultMul2, (a * 2.0f));
}

TEST(Vector2, DivideVector2)
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(2.0f, 4.0f);

    ASSERT_EQ(resultDiv, (a / b));
}

TEST(Vector2, DivideFloat)
{
    Vector2 a(1.0f, 2.0f);

    ASSERT_EQ(resultDiv2, (a / 2.0f));
}

TEST(Vector2, Dot)
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(2.0f, 3.0f);

    ASSERT_EQ(8.0f, a.Dot(b));
}

TEST(Vector2, Length)
{
    Vector2 a(2.0f, 0.0f);

    ASSERT_EQ(2.0f, a.Length());
}

TEST(Vector2, Normalize)
{
    Vector2 a(2.0f, 0.0f);
    ASSERT_EQ(resultNorm, a.Normalize());
}

TEST(Vector2, Equal)
{
    Vector2 a(1.0f);
    Vector2 b(1.0f);
    Vector2 c(1.0f, 1.1f);

    ASSERT_TRUE(a == b);
    ASSERT_FALSE(a == c);
}

TEST(Vector2, NotEqual)
{
    Vector2 a(1.0f);
    Vector2 b(1.0f, 1.1f);
    Vector2 c(1.0f);

    ASSERT_TRUE(a != b);
    ASSERT_FALSE(a != c);
}

TEST(Vector2, CastToVector3)
{
    Vector2 v1(vectorToCast);
    Vector3 v2 = static_cast<Vector3>(v1);

    ASSERT_EQ(v2, resultCastV3);
}

TEST(Vector2, CastToVector4)
{
    Vector2 v1(vectorToCast);
    Vector4 v2 = static_cast<Vector4>(v1);

    ASSERT_EQ(v2, resultCastV4);
}
