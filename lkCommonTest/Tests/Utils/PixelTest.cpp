#include <gtest/gtest.h>
#include <lkCommon/Utils/Pixel.hpp>
#include <cstdint>
#include <iostream>

using namespace lkCommon::Utils;

const uint8_t TEST_PIXEL_RAW_UINT[] = {10, 30, 20, 15};
const uint8_t TEST_PIXEL_RAW_UINT_2[] = {20, 10, 255, 99};
const float TEST_PIXEL_RAW_FLOAT[] = { 10.0f / 255.0f, 30.0f / 255.0f, 20.0f / 255.0f, 15.0f / 255.0f };
const float TEST_PIXEL_RAW_FLOAT_2[] = { 20.0f / 255.0f, 10.0f / 255.0f, 255.0f / 255.0f, 99.0f / 255.0f };

const Pixel<uint8_t, 4> TEST_PIXEL_UINT_ZEROS(static_cast<uint8_t>(0));
const Pixel<uint8_t, 4> TEST_PIXEL_UINT_MAX(255);
const Pixel<uint8_t, 4> TEST_PIXEL_UINT(TEST_PIXEL_RAW_UINT);
const Pixel<uint8_t, 4> TEST_PIXEL_UINT_2(TEST_PIXEL_RAW_UINT_2);
const Pixel<float, 4> TEST_PIXEL_FLOAT_ZEROS(0.0f);
const Pixel<float, 4> TEST_PIXEL_FLOAT_MAX(1.0f);
const Pixel<float, 4> TEST_PIXEL_FLOAT(TEST_PIXEL_RAW_FLOAT);
const Pixel<float, 4> TEST_PIXEL_FLOAT_2(TEST_PIXEL_RAW_FLOAT_2);

TEST(Pixel, DefaultConstructor)
{
    Pixel<uint8_t, 4> pixelUint;
    Pixel<float, 4> pixelFloat;

    EXPECT_EQ(TEST_PIXEL_UINT_ZEROS, pixelUint);
    EXPECT_EQ(TEST_PIXEL_FLOAT_ZEROS, pixelFloat);
}

TEST(Pixel, ConstructorSingleArgument)
{
    Pixel<uint8_t, 4> pixelUint(255);
    Pixel<float, 4> pixelFloat(1.0f);

    EXPECT_EQ(TEST_PIXEL_UINT_MAX, pixelUint);
    EXPECT_EQ(TEST_PIXEL_FLOAT_MAX, pixelFloat);
    EXPECT_NE(TEST_PIXEL_UINT_ZEROS, pixelUint);
    EXPECT_NE(TEST_PIXEL_FLOAT_ZEROS, pixelFloat);
}

TEST(Pixel, ConstructorArray)
{
    Pixel<uint8_t, 4> pixelUint(TEST_PIXEL_RAW_UINT);
    Pixel<float, 4> pixelFloat(TEST_PIXEL_RAW_FLOAT);

    EXPECT_EQ(TEST_PIXEL_UINT, pixelUint);
    EXPECT_EQ(TEST_PIXEL_FLOAT, pixelFloat);
    EXPECT_NE(TEST_PIXEL_UINT_ZEROS, pixelUint);
    EXPECT_NE(TEST_PIXEL_FLOAT_ZEROS, pixelFloat);
}

TEST(Pixel, ConstructorFloatOverMax)
{
    Pixel<float, 4> pixelFloat(2.0f);
    EXPECT_EQ(TEST_PIXEL_FLOAT_MAX, pixelFloat);
}

TEST(Pixel, ConstructorFloatBelowZero)
{
    Pixel<float, 4> pixelFloat(-1.0f);
    EXPECT_EQ(TEST_PIXEL_FLOAT_ZEROS, pixelFloat);
}

TEST(Pixel, CastUintToFloat)
{
    Pixel<float, 4> PIXEL_FLOAT = TEST_PIXEL_UINT;
    Pixel<float, 4> PIXEL_FLOAT_2 = TEST_PIXEL_UINT_2;

    EXPECT_EQ(TEST_PIXEL_FLOAT, PIXEL_FLOAT);
    EXPECT_EQ(TEST_PIXEL_FLOAT_2, PIXEL_FLOAT_2);
    EXPECT_NE(PIXEL_FLOAT, PIXEL_FLOAT_2);
}

TEST(Pixel, CastFloatToUint)
{
    Pixel<uint8_t, 4> PIXEL_UINT = TEST_PIXEL_FLOAT;
    Pixel<uint8_t, 4> PIXEL_UINT_2 = TEST_PIXEL_FLOAT_2;

    EXPECT_EQ(TEST_PIXEL_UINT, PIXEL_UINT);
    EXPECT_EQ(TEST_PIXEL_UINT_2, PIXEL_UINT_2);
    EXPECT_NE(PIXEL_UINT, PIXEL_UINT_2);
}
