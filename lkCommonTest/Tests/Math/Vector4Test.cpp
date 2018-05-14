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
    EXPECT_TRUE(resultZero == a);
}

TEST(Vector4, ConstructorSingleFloat)
{
    Vector4 a(1.0f);

    EXPECT_TRUE(resultOne == a);
}

TEST(Vector4, AddVector4)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 3.0f, 4.0f, 5.0f);

    EXPECT_TRUE(resultAdd == (a + b));
}

TEST(Vector4, AddFloat)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);

    EXPECT_TRUE(resultAdd2 == (a + 2.0f));
}

TEST(Vector4, SubtractVector4)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 3.0f, 4.0f, 5.0f);

    EXPECT_TRUE(resultSub == (a - b));
}

TEST(Vector4, SubtractFloat)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);

    EXPECT_TRUE(resultSub2 == (a - 2.0f));
}

TEST(Vector4, MultiplyVector4)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 3.0f, 4.0f, 5.0f);

    EXPECT_TRUE(resultMul == (a * b));
}

TEST(Vector4, MultiplyFloat)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);

    EXPECT_TRUE(resultMul2 == (a * 2.0f));
}

TEST(Vector4, DivideVector4)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 4.0f, 6.0f, 8.0f);

    EXPECT_TRUE(resultDiv == (a / b));
}

TEST(Vector4, DivideFloat)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);

    EXPECT_TRUE(resultDiv2 == (a / 2.0f));
}

TEST(Vector4, Dot)
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 3.0f, 4.0f, 5.0f);

    EXPECT_EQ(40.0f, a.Dot(b));
}

TEST(Vector4, Cross)
{
    Vector4 a(1.0f, 0.0f, 0.0f, 0.0f);
    Vector4 b(0.0f, 1.0f, 0.0f, 0.0f);

    EXPECT_TRUE(resultCross == a.Cross(b));
}

TEST(Vector4, Length)
{
    Vector4 a(2.0f, 0.0f, 0.0f, 0.0f);

    EXPECT_EQ(2.0f, a.Length());
}

TEST(Vector4, Normalize)
{
    Vector4 a(2.0f, 0.0f, 0.0f, 0.0f);
    a.Normalize();

    EXPECT_TRUE(resultNorm == a);
}

TEST(Vector4, Equal)
{
    Vector4 a(1.0f);

    EXPECT_TRUE(a == a);
}

TEST(Vector4, Greater)
{
    Vector4 a(1.0f);
    Vector4 b(2.0f);

    ASSERT_GT(b, a);
}

TEST(Vector4, GreaterEqual)
{
    Vector4 a(1.0f);
    Vector4 b(2.0f);
    Vector4 c(1.0f);

    ASSERT_GE(b, a);
    ASSERT_GE(c, a);
}

TEST(Vector4, Less)
{
    Vector4 a(1.0f);
    Vector4 b(2.0f);

    ASSERT_LT(a, b);
}

TEST(Vector4, LessEqual)
{
    Vector4 a(1.0f);
    Vector4 b(2.0f);
    Vector4 c(1.0f);

    ASSERT_LE(a, b);
    ASSERT_LE(a, c);
}
