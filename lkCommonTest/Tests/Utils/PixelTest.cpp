#include <gtest/gtest.h>
#include <lkCommon/Utils/Pixel.hpp>
#include <cstdint>
#include <iostream>

using namespace lkCommon::Utils;

const uint8_t TEST_PIXEL_RAW_UINT[4] =          {  10,  30,  20,  15 };
const uint8_t TEST_PIXEL_RAW_UINT_2[4] =        {  20,  10, 255,  99 };

const float TEST_PIXEL_RAW_FLOAT[4] =           {  10.0f / 255.0f,  30.0f / 255.0f,  20.0f / 255.0f,  15.0f / 255.0f };
const float TEST_PIXEL_RAW_FLOAT_2[4] =         {  20.0f / 255.0f,  10.0f / 255.0f, 255.0f / 255.0f,  99.0f / 255.0f };

const PixelUint4 TEST_PIXEL_UINT_ZEROS(static_cast<uint8_t>(0));
const PixelUint4 TEST_PIXEL_UINT_MAX(255);
const PixelUint4 TEST_PIXEL_UINT(TEST_PIXEL_RAW_UINT);
const PixelUint4 TEST_PIXEL_UINT_2(TEST_PIXEL_RAW_UINT_2);

const PixelFloat4 TEST_PIXEL_FLOAT_ZEROS(0.0f);
const PixelFloat4 TEST_PIXEL_FLOAT_MAX(1.0f);
const PixelFloat4 TEST_PIXEL_FLOAT(TEST_PIXEL_RAW_FLOAT);
const PixelFloat4 TEST_PIXEL_FLOAT_2(TEST_PIXEL_RAW_FLOAT_2);


const uint8_t TEST_CONSTANT_UINT = 2;
const PixelUint4 TEST_PIXEL_UINT_3              ({  20,  30,  40,  24 });
const PixelUint4 TEST_PIXEL_UINT_4              ({  10,   5,   4,   2 });
const PixelUint4 TEST_PIXEL_UINT_ADD_3_4        ({  30,  35,  44,  26 });
const PixelUint4 TEST_PIXEL_UINT_SUB_3_4        ({  10,  25,  36,  22 });
const PixelUint4 TEST_PIXEL_UINT_MUL_3_4        ({ 200, 150, 160,  48 });
const PixelUint4 TEST_PIXEL_UINT_DIV_3_4        ({   2,   6,  10,  12 });
const PixelUint4 TEST_PIXEL_UINT_ADD_3_CONSTANT ({  22,  32,  42,  26 });
const PixelUint4 TEST_PIXEL_UINT_SUB_3_CONSTANT ({  18,  28,  38,  22 });
const PixelUint4 TEST_PIXEL_UINT_MUL_3_CONSTANT ({  40,  60,  80,  48 });
const PixelUint4 TEST_PIXEL_UINT_DIV_3_CONSTANT ({  10,  15,  20,  12 });

const float TEST_ABS_ERROR = 0.000001f;
const float TEST_CONSTANT_FLOAT = 2.0f;
const PixelFloat4 TEST_PIXEL_FLOAT_3                ({  20.0f,  30.0f,  40.0f,  24.0f });
const PixelFloat4 TEST_PIXEL_FLOAT_4                ({  10.0f,   5.0f,   4.0f,   2.0f });
const PixelFloat4 TEST_PIXEL_FLOAT_ADD_3_4          ({  30.0f,  35.0f,  44.0f,  26.0f });
const PixelFloat4 TEST_PIXEL_FLOAT_SUB_3_4          ({  10.0f,  25.0f,  36.0f,  22.0f });
const PixelFloat4 TEST_PIXEL_FLOAT_MUL_3_4          ({ 200.0f, 150.0f, 160.0f,  48.0f });
const PixelFloat4 TEST_PIXEL_FLOAT_DIV_3_4          ({   2.0f,   6.0f,  10.0f,  12.0f });
const PixelFloat4 TEST_PIXEL_FLOAT_ADD_3_CONSTANT   ({  22.0f,  32.0f,  42.0f,  26.0f });
const PixelFloat4 TEST_PIXEL_FLOAT_SUB_3_CONSTANT   ({  18.0f,  28.0f,  38.0f,  22.0f });
const PixelFloat4 TEST_PIXEL_FLOAT_MUL_3_CONSTANT   ({  40.0f,  60.0f,  80.0f,  48.0f });
const PixelFloat4 TEST_PIXEL_FLOAT_DIV_3_CONSTANT   ({  10.0f,  15.0f,  20.0f,  12.0f });


const float TEST_CONSTANT_CLAMP_FLOAT_POSITIVE = 2.0f;
const float TEST_CONSTANT_CLAMP_FLOAT_NEGATIVE = -0.5f;
const float TEST_CONSTANT_CLAMP_UINT = 255;


TEST(Pixel, DefaultConstructor)
{
    PixelUint4 pixelUint;
    PixelFloat4 pixelFloat;

    EXPECT_EQ(TEST_PIXEL_UINT_ZEROS, pixelUint);
    EXPECT_EQ(TEST_PIXEL_FLOAT_ZEROS, pixelFloat);
}

TEST(Pixel, ConstructorSingleArgument)
{
    PixelUint4 pixelUint(255);
    PixelFloat4 pixelFloat(1.0f);

    EXPECT_EQ(TEST_PIXEL_UINT_MAX, pixelUint);
    EXPECT_EQ(TEST_PIXEL_FLOAT_MAX, pixelFloat);
    EXPECT_NE(TEST_PIXEL_UINT_ZEROS, pixelUint);
    EXPECT_NE(TEST_PIXEL_FLOAT_ZEROS, pixelFloat);
}

TEST(Pixel, ConstructorArray)
{
    PixelUint4 pixelUint(TEST_PIXEL_RAW_UINT);
    PixelFloat4 pixelFloat(TEST_PIXEL_RAW_FLOAT);

    EXPECT_EQ(TEST_PIXEL_UINT, pixelUint);
    EXPECT_EQ(TEST_PIXEL_FLOAT, pixelFloat);
    EXPECT_NE(TEST_PIXEL_UINT_ZEROS, pixelUint);
    EXPECT_NE(TEST_PIXEL_FLOAT_ZEROS, pixelFloat);
}

TEST(Pixel, ConstructorInitializerList)
{
    PixelUint4 pixelUint({10, 30, 20, 15});
    PixelFloat4 pixelFloat = { 10.0f / 255.0f, 30.0f / 255.0f, 20.0f / 255.0f, 15.0f / 255.0f };

    EXPECT_EQ(TEST_PIXEL_UINT, pixelUint);
    EXPECT_EQ(TEST_PIXEL_FLOAT, pixelFloat);
}

TEST(Pixel, ConstructorFloatOverMax)
{
    PixelFloat4 pixelFloat(2.0f);
    EXPECT_EQ(TEST_PIXEL_FLOAT_MAX, pixelFloat);
}

TEST(Pixel, ConstructorFloatBelowZero)
{
    PixelFloat4 pixelFloat(-1.0f);
    EXPECT_EQ(TEST_PIXEL_FLOAT_ZEROS, pixelFloat);
}

TEST(Pixel, CastUintToFloat)
{
    PixelFloat4 PIXEL_FLOAT = TEST_PIXEL_UINT;
    PixelFloat4 PIXEL_FLOAT_2 = TEST_PIXEL_UINT_2;

    EXPECT_EQ(TEST_PIXEL_FLOAT, PIXEL_FLOAT);
    EXPECT_EQ(TEST_PIXEL_FLOAT_2, PIXEL_FLOAT_2);
    EXPECT_NE(PIXEL_FLOAT, PIXEL_FLOAT_2);
}

TEST(Pixel, CastFloatToUint)
{
    PixelUint4 PIXEL_UINT = TEST_PIXEL_FLOAT;
    PixelUint4 PIXEL_UINT_2 = TEST_PIXEL_FLOAT_2;

    EXPECT_EQ(TEST_PIXEL_UINT, PIXEL_UINT);
    EXPECT_EQ(TEST_PIXEL_UINT_2, PIXEL_UINT_2);
    EXPECT_NE(PIXEL_UINT, PIXEL_UINT_2);
}

TEST(Pixel, AddPixelUint8)
{
    PixelUint4 val1 = TEST_PIXEL_UINT_3;
    PixelUint4 val2 = TEST_PIXEL_UINT_4;
    EXPECT_EQ(TEST_PIXEL_UINT_ADD_3_4, val1 + val2);
}

TEST(Pixel, SubPixelUint8)
{
    PixelUint4 val1 = TEST_PIXEL_UINT_3;
    PixelUint4 val2 = TEST_PIXEL_UINT_4;
    EXPECT_EQ(TEST_PIXEL_UINT_SUB_3_4, val1 - val2);
}

TEST(Pixel, MulPixelUint8)
{
    PixelUint4 val1 = TEST_PIXEL_UINT_3;
    PixelUint4 val2 = TEST_PIXEL_UINT_4;
    EXPECT_EQ(TEST_PIXEL_UINT_MUL_3_4, val1 * val2);
}

TEST(Pixel, DivPixelUint8)
{
    PixelUint4 val1 = TEST_PIXEL_UINT_3;
    PixelUint4 val2 = TEST_PIXEL_UINT_4;
    EXPECT_EQ(TEST_PIXEL_UINT_DIV_3_4, val1 / val2);
}

TEST(Pixel, AddConstantUint8)
{
    PixelUint4 val1 = TEST_PIXEL_UINT_3;
    uint8_t val2 = TEST_CONSTANT_UINT;
    EXPECT_EQ(TEST_PIXEL_UINT_ADD_3_CONSTANT, val1 + val2);
}

TEST(Pixel, SubConstantUint8)
{
    PixelUint4 val1 = TEST_PIXEL_UINT_3;
    uint8_t val2 = TEST_CONSTANT_UINT;
    EXPECT_EQ(TEST_PIXEL_UINT_SUB_3_CONSTANT, val1 - val2);
}

TEST(Pixel, MulConstantUint8)
{
    PixelUint4 val1 = TEST_PIXEL_UINT_3;
    uint8_t val2 = TEST_CONSTANT_UINT;
    EXPECT_EQ(TEST_PIXEL_UINT_MUL_3_CONSTANT, val1 * val2);
}

TEST(Pixel, DivConstantUint8)
{
    PixelUint4 val1 = TEST_PIXEL_UINT_3;
    uint8_t val2 = TEST_CONSTANT_UINT;
    EXPECT_EQ(TEST_PIXEL_UINT_DIV_3_CONSTANT, val1 / val2);
}

TEST(Pixel, AddPixelFloat)
{
    PixelFloat4 val1 = TEST_PIXEL_FLOAT_3;
    PixelFloat4 val2 = TEST_PIXEL_FLOAT_4;
    PixelFloat4 result = val1 + val2;
    EXPECT_NEAR(TEST_PIXEL_FLOAT_ADD_3_4[0], result[0], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_ADD_3_4[1], result[1], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_ADD_3_4[2], result[2], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_ADD_3_4[3], result[3], TEST_ABS_ERROR);
}

TEST(Pixel, SubPixelFloat)
{
    PixelFloat4 val1 = TEST_PIXEL_FLOAT_3;
    PixelFloat4 val2 = TEST_PIXEL_FLOAT_4;
    PixelFloat4 result = val1 - val2;
    EXPECT_NEAR(TEST_PIXEL_FLOAT_SUB_3_4[0], result[0], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_SUB_3_4[1], result[1], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_SUB_3_4[2], result[2], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_SUB_3_4[3], result[3], TEST_ABS_ERROR);
}

TEST(Pixel, MulPixelFloat)
{
    PixelFloat4 val1 = TEST_PIXEL_FLOAT_3;
    PixelFloat4 val2 = TEST_PIXEL_FLOAT_4;
    PixelFloat4 result = val1 * val2;
    EXPECT_NEAR(TEST_PIXEL_FLOAT_MUL_3_4[0], result[0], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_MUL_3_4[1], result[1], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_MUL_3_4[2], result[2], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_MUL_3_4[3], result[3], TEST_ABS_ERROR);
}

TEST(Pixel, DivPixelFloat)
{
    PixelFloat4 val1 = TEST_PIXEL_FLOAT_3;
    PixelFloat4 val2 = TEST_PIXEL_FLOAT_4;
    PixelFloat4 result = val1 / val2;
    EXPECT_NEAR(TEST_PIXEL_FLOAT_DIV_3_4[0], result[0], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_DIV_3_4[1], result[1], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_DIV_3_4[2], result[2], TEST_ABS_ERROR);
    EXPECT_NEAR(TEST_PIXEL_FLOAT_DIV_3_4[3], result[3], TEST_ABS_ERROR);
}

TEST(Pixel, AddConstantFloat)
{
    PixelFloat4 val1 = TEST_PIXEL_FLOAT_3;
    float val2 = TEST_CONSTANT_FLOAT;
    EXPECT_EQ(TEST_PIXEL_FLOAT_ADD_3_CONSTANT, val1 + val2);
}

TEST(Pixel, SubConstantFloat)
{
    PixelFloat4 val1 = TEST_PIXEL_FLOAT_3;
    float val2 = TEST_CONSTANT_FLOAT;
    EXPECT_EQ(TEST_PIXEL_FLOAT_SUB_3_CONSTANT, val1 - val2);
}

TEST(Pixel, MulConstantFloat)
{
    PixelFloat4 val1 = TEST_PIXEL_FLOAT_3;
    float val2 = TEST_CONSTANT_FLOAT;
    EXPECT_EQ(TEST_PIXEL_FLOAT_MUL_3_CONSTANT, val1 * val2);
}

TEST(Pixel, DivConstantFloat)
{
    PixelFloat4 val1 = TEST_PIXEL_FLOAT_3;
    float val2 = TEST_CONSTANT_FLOAT;
    EXPECT_EQ(TEST_PIXEL_FLOAT_DIV_3_CONSTANT, val1 / val2);
}

TEST(Pixel, FloatToUintCastClampTest)
{
    PixelFloat4 val1(TEST_CONSTANT_CLAMP_FLOAT_POSITIVE);
    PixelUint4 val2 = static_cast<PixelUint4>(val1);
    EXPECT_EQ(TEST_PIXEL_UINT_MAX, val2);

    PixelFloat4 val3(TEST_CONSTANT_CLAMP_FLOAT_NEGATIVE);
    val2 = static_cast<PixelUint4>(val3);
    EXPECT_EQ(TEST_PIXEL_UINT_ZEROS, val2);
}

TEST(Pixel, UintToFloatCastClampTest)
{
    PixelUint4 val1(TEST_CONSTANT_CLAMP_UINT);
    PixelFloat4 val2 = static_cast<PixelFloat4>(val1);
    EXPECT_EQ(TEST_PIXEL_FLOAT_MAX, val2);
}
