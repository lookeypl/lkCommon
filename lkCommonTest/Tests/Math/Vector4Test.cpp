#include <gtest/gtest.h>

#include <lkCommon/Math/Vector4.hpp>

using namespace lkCommon::Math;


namespace {

const Vector4 resultZero(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 resultOne(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 resultAdd(3.0f, 5.0f, 7.0f, 9.0f);
const Vector4 resultAdd2(3.0f, 4.0f, 5.0f, 6.0f);
const Vector4 resultSub(-1.0f, -1.0f, -1.0f, -1.0f);
const Vector4 resultSub2(-1.0f, 0.0f, 1.0f, 2.0f);
const Vector4 resultMul(2.0f, 6.0f, 12.0f, 20.0f);
const Vector4 resultMul2(2.0f, 4.0f, 6.0f, 8.0f);
const Vector4 resultDiv(0.5f, 0.5f, 0.5f, 0.5f);
const Vector4 resultDiv2(0.5f, 1.0f, 1.5f, 2.0f);
const Vector4 resultPow(8.0f);
const Vector4 resultCross(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4 resultNorm(1.0f, 0.0f, 0.0f, 0.0f);

} // namespace

TEST(Vector4, Constructor)
{
    Vector4 a;
    ASSERT_EQ(resultZero, a);
}

TEST(Vector4, ConstructorSingleFloat)
{
    Vector4 a(1.0f);
    ASSERT_EQ(resultOne, a);
}

TEST(Vector4, CopyConstructor)
{
    Vector4 v(resultOne);
    ASSERT_EQ(v, resultOne);
}

TEST(Vector4, MoveConstructor)
{
    Vector4 v(resultOne);
    Vector4 v2(std::move(v));
    ASSERT_EQ(v2, resultOne);
}

TEST(Vector4, CopySubstitution)
{
    Vector4 v(2.0f);
    ASSERT_NE(v, resultOne);
    v = resultOne;
    ASSERT_EQ(v, resultOne);
}

TEST(Vector4, MoveSubstitution)
{
    Vector4 v(resultOne);
    Vector4 v2(2.0f);
    ASSERT_NE(v, v2);
    v2 = std::move(v);
    ASSERT_EQ(v2, resultOne);
}

TEST(Vector4, AddVector4)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 3.0f, 4.0f, 5.0f);

    ASSERT_EQ(resultAdd, (a + b));
}

TEST(Vector4, AddFloat)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);

    ASSERT_EQ(resultAdd2, (a + 2.0f));
}

TEST(Vector4, SubtractVector4)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 3.0f, 4.0f, 5.0f);

    ASSERT_EQ(resultSub, (a - b));
}

TEST(Vector4, SubtractFloat)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);

    ASSERT_EQ(resultSub2, (a - 2.0f));
}

TEST(Vector4, MultiplyVector4)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 3.0f, 4.0f, 5.0f);

    ASSERT_EQ(resultMul, (a * b));
}

TEST(Vector4, MultiplyFloat)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);

    ASSERT_EQ(resultMul2, (a * 2.0f));
}

TEST(Vector4, DivideVector4)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 4.0f, 6.0f, 8.0f);

    ASSERT_EQ(resultDiv, (a / b));
}

TEST(Vector4, DivideFloat)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);

    ASSERT_EQ(resultDiv2, (a / 2.0f));
}

TEST(Vector4, Dot)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 3.0f, 4.0f, 5.0f);

    ASSERT_EQ(40.0f, a.Dot(b));
}

TEST(Vector4, Cross)
{
    Vector4 a(1.0f, 0.0f, 0.0f, 0.0f);
    Vector4 b(0.0f, 1.0f, 0.0f, 0.0f);

    ASSERT_EQ(resultCross, a.Cross(b));
}

TEST(Vector4, Length)
{
    Vector4 a(2.0f, 0.0f, 0.0f, 0.0f);

    ASSERT_EQ(2.0f, a.Length());
}

TEST(Vector4, Normalize)
{
    Vector4 a(2.0f, 0.0f, 0.0f, 0.0f);
    Vector4 norm = a.Normalize();
    const double EPSILON = 0.001;

    EXPECT_NEAR(resultNorm[0], norm[0], EPSILON);
    EXPECT_NEAR(resultNorm[1], norm[1], EPSILON);
    EXPECT_NEAR(resultNorm[2], norm[2], EPSILON);
    EXPECT_NEAR(resultNorm[3], norm[3], EPSILON);
}

TEST(Vector4, Equal)
{
    Vector4 a(1.0f);
    Vector4 b(1.0f);
    Vector4 c(1.0f, 1.0f, 1.0f, 1.1f);

    ASSERT_TRUE(a == b);
    ASSERT_FALSE(a == c);
}

TEST(Vector4, NotEqual)
{
    Vector4 a(1.0f);
    Vector4 b(1.0f, 1.0f, 1.0f, 1.1f);
    Vector4 c(1.0f);

    ASSERT_TRUE(a != b);
    ASSERT_FALSE(a != c);
}
