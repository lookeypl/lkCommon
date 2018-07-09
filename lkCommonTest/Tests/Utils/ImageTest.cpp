#include <gtest/gtest.h>
#include <lkCommon/Utils/Image.hpp>

const uint32_t TEST_WIDTH = 10;
const uint32_t TEST_HEIGHT = 20;

const uint8_t TEST_PIXEL_RAW[] = {10, 30, 20, 15};
const uint8_t TEST_PIXEL_RAW_2[] = {20, 10, 18, 99};

const lkCommon::Utils::Pixel<uint8_t, 4> TEST_PIXEL(TEST_PIXEL_RAW);
const lkCommon::Utils::Pixel<uint8_t, 4> TEST_PIXEL_2(TEST_PIXEL_RAW_2);

bool CompareRawPixels(const uint8_t p1[4], const uint8_t p2[4], bool withAlpha)
{
    return (p1[2] == p2[2]) && (p1[1] == p2[1]) && (p1[0] == p2[0]) && (withAlpha ? (p1[3] == p2[3]) : true);
}

TEST(Image, ResizeTest)
{
    lkCommon::Utils::Image i;
    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
}

TEST(Image, GetDimensions)
{
    lkCommon::Utils::Image i;
    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_EQ(i.GetWidth(), TEST_WIDTH);
    EXPECT_EQ(i.GetHeight(), TEST_HEIGHT);
}

TEST(Image, GetDataPointer)
{
    lkCommon::Utils::Image i;

    EXPECT_EQ(i.GetDataPtr(), nullptr);
    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_NE(i.GetDataPtr(), nullptr);
}

TEST(Image, SetPixel)
{
    lkCommon::Utils::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL_RAW[2], TEST_PIXEL_RAW[1], TEST_PIXEL_RAW[0]));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL_RAW[2], TEST_PIXEL_RAW[1], TEST_PIXEL_RAW[0], TEST_PIXEL_RAW[3]));

    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL_RAW[2], TEST_PIXEL_RAW[1], TEST_PIXEL_RAW[0]));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL_RAW[2], TEST_PIXEL_RAW[1], TEST_PIXEL_RAW[0], TEST_PIXEL_RAW[3]));
}

TEST(Image, SetPixelOutOfBounds)
{
    lkCommon::Utils::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_FALSE(i.SetPixel(TEST_WIDTH, 0, TEST_PIXEL));
    EXPECT_FALSE(i.SetPixel(TEST_WIDTH, 0, TEST_PIXEL_RAW[2], TEST_PIXEL_RAW[1], TEST_PIXEL_RAW[0]));
    EXPECT_FALSE(i.SetPixel(TEST_WIDTH, 0, TEST_PIXEL_RAW[2], TEST_PIXEL_RAW[1], TEST_PIXEL_RAW[0], TEST_PIXEL_RAW[3]));

    EXPECT_FALSE(i.SetPixel(0, TEST_HEIGHT, TEST_PIXEL));
    EXPECT_FALSE(i.SetPixel(0, TEST_HEIGHT, TEST_PIXEL_RAW[2], TEST_PIXEL_RAW[1], TEST_PIXEL_RAW[0]));
    EXPECT_FALSE(i.SetPixel(0, TEST_HEIGHT, TEST_PIXEL_RAW[2], TEST_PIXEL_RAW[1], TEST_PIXEL_RAW[0], TEST_PIXEL_RAW[3]));
}

TEST(Image, GetPixel)
{
    lkCommon::Utils::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL_2));

    lkCommon::Utils::Pixel<uint8_t, 4> r1;
    lkCommon::Utils::Pixel<uint8_t, 4> r2;
    EXPECT_TRUE(i.GetPixel(0, 0, r1));
    EXPECT_TRUE(i.GetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, r2));

    EXPECT_EQ(r1, TEST_PIXEL);
    EXPECT_EQ(r2, TEST_PIXEL_2);
    EXPECT_NE(r1, r2);
}

TEST(Image, GetPixelRGB)
{
    lkCommon::Utils::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL_2));

    uint8_t pixel1[3];
    uint8_t pixel2[3];
    EXPECT_TRUE(i.GetPixel(0, 0, pixel1[2], pixel1[1], pixel1[0]));
    EXPECT_TRUE(i.GetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, pixel2[2], pixel2[1], pixel2[0]));

    EXPECT_TRUE(CompareRawPixels(pixel1, TEST_PIXEL_RAW, false));
    EXPECT_TRUE(CompareRawPixels(pixel2, TEST_PIXEL_RAW_2, false));
    EXPECT_FALSE(CompareRawPixels(pixel1, pixel2, false));
}

TEST(Image, GetPixelRGBA)
{
    lkCommon::Utils::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL_2));

    lkCommon::Utils::Pixel<uint8_t, 4> r1;
    lkCommon::Utils::Pixel<uint8_t, 4> r2;
    EXPECT_TRUE(i.GetPixel(0, 0, r1.mColors[2], r1.mColors[1], r1.mColors[0], r1.mColors[3]));
    EXPECT_TRUE(i.GetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, r2.mColors[2], r2.mColors[1], r2.mColors[0], r2.mColors[3]));

    EXPECT_EQ(r1, TEST_PIXEL);
    EXPECT_EQ(r2, TEST_PIXEL_2);
    EXPECT_NE(r1, r2);
}

TEST(Image, GetPixelOutOfBounds)
{
    lkCommon::Utils::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));

    lkCommon::Utils::Pixel<uint8_t, 4> r1;
    EXPECT_FALSE(i.GetPixel(TEST_WIDTH, TEST_HEIGHT, r1));
    EXPECT_FALSE(i.GetPixel(TEST_WIDTH, TEST_HEIGHT, r1.mColors[2], r1.mColors[1], r1.mColors[0]));
    EXPECT_FALSE(i.GetPixel(TEST_WIDTH, TEST_HEIGHT, r1.mColors[2], r1.mColors[1], r1.mColors[0], r1.mColors[3]));
}
