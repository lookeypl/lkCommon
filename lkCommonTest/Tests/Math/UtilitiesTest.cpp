#include <gtest/gtest.h>
#include <lkCommon/Math/Utilities.hpp>

#include <lkCommon/Math/Vector4.hpp>

const float TEST_FLOAT_A = 1.0f;
const float TEST_FLOAT_B = 3.0f;
const float TEST_FLOAT_FACTOR = 0.5f;
const float TEST_FLOAT_RESULT = 2.0f;

const lkCommon::Math::Vector4 TEST_VECTOR_A(1.0f, 2.0f, 3.0f, 4.0f);
const lkCommon::Math::Vector4 TEST_VECTOR_B(3.0f, 4.0f, 7.0f, 6.0f);
const float TEST_VECTOR_FACTOR = 0.5f;
const lkCommon::Math::Vector4 TEST_VECTOR_RESULT(2.0f, 3.0f, 5.0f, 5.0f);

TEST(MathUtil, LerpFloat)
{
    EXPECT_EQ(TEST_FLOAT_RESULT, lkCommon::Math::Util::Lerp(TEST_FLOAT_A, TEST_FLOAT_B, TEST_FLOAT_FACTOR));
}

TEST(MathUtil, LerpVector4)
{
    EXPECT_EQ(TEST_VECTOR_RESULT, lkCommon::Math::Util::Lerp(TEST_VECTOR_A, TEST_VECTOR_B, TEST_VECTOR_FACTOR));
}
