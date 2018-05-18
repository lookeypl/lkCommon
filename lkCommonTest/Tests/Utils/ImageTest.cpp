#include <gtest/gtest.h>
#include <lkCommon/Utils/Image.hpp>

const uint32_t TEST_WIDTH = 10;
const uint32_t TEST_HEIGHT = 20;
const lkCommon::Image::Pixel TEST_PIXEL(10, 30, 20, 15);
const lkCommon::Image::Pixel TEST_PIXEL_2(20, 10, 18, 99);

bool ComparePixels(const lkCommon::Image::Pixel& p1, const lkCommon::Image::Pixel& p2, bool withAlpha)
{
    return (p1.r == p2.r) && (p1.g == p2.g) && (p1.b == p2.b) && (withAlpha ? (p1.a == p2.a) : true);
}

TEST(Image, ResizeTest)
{
    lkCommon::Image i;
    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
}

TEST(Image, SetPixel)
{
    lkCommon::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL.r, TEST_PIXEL.g, TEST_PIXEL.b));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL.r, TEST_PIXEL.g, TEST_PIXEL.b, TEST_PIXEL.a));

    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL.r, TEST_PIXEL.g, TEST_PIXEL.b));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL.r, TEST_PIXEL.g, TEST_PIXEL.b, TEST_PIXEL.a));
}

TEST(Image, SetPixelOutOfBounds)
{
    lkCommon::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_FALSE(i.SetPixel(TEST_WIDTH, 0, TEST_PIXEL));
    EXPECT_FALSE(i.SetPixel(TEST_WIDTH, 0, TEST_PIXEL.r, TEST_PIXEL.g, TEST_PIXEL.b));
    EXPECT_FALSE(i.SetPixel(TEST_WIDTH, 0, TEST_PIXEL.r, TEST_PIXEL.g, TEST_PIXEL.b, TEST_PIXEL.a));

    EXPECT_FALSE(i.SetPixel(0, TEST_HEIGHT, TEST_PIXEL));
    EXPECT_FALSE(i.SetPixel(0, TEST_HEIGHT, TEST_PIXEL.r, TEST_PIXEL.g, TEST_PIXEL.b));
    EXPECT_FALSE(i.SetPixel(0, TEST_HEIGHT, TEST_PIXEL.r, TEST_PIXEL.g, TEST_PIXEL.b, TEST_PIXEL.a));
}

TEST(Image, GetPixelStruct)
{
    lkCommon::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL_2));

    lkCommon::Image::Pixel r1;
    lkCommon::Image::Pixel r2;
    EXPECT_TRUE(i.GetPixel(0, 0, r1));
    EXPECT_TRUE(i.GetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, r2));

    EXPECT_TRUE(ComparePixels(r1, TEST_PIXEL, true));
    EXPECT_TRUE(ComparePixels(r2, TEST_PIXEL_2, true));
    EXPECT_FALSE(ComparePixels(r1, r2, true));
}

TEST(Image, GetPixelRGB)
{
    lkCommon::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL_2));

    lkCommon::Image::Pixel r1;
    lkCommon::Image::Pixel r2;
    EXPECT_TRUE(i.GetPixel(0, 0, r1.r, r1.g, r1.b));
    EXPECT_TRUE(i.GetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, r2.r, r2.g, r2.b));

    EXPECT_TRUE(ComparePixels(r1, TEST_PIXEL, false));
    EXPECT_TRUE(ComparePixels(r2, TEST_PIXEL_2, false));
    EXPECT_FALSE(ComparePixels(r1, r2, false));
}

TEST(Image, GetPixelRGBA)
{
    lkCommon::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL_2));

    lkCommon::Image::Pixel r1;
    lkCommon::Image::Pixel r2;
    EXPECT_TRUE(i.GetPixel(0, 0, r1.r, r1.g, r1.b, r1.a));
    EXPECT_TRUE(i.GetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, r2.r, r2.g, r2.b, r2.a));

    EXPECT_TRUE(ComparePixels(r1, TEST_PIXEL, true));
    EXPECT_TRUE(ComparePixels(r2, TEST_PIXEL_2, true));
    EXPECT_FALSE(ComparePixels(r1, r2, true));
}

TEST(Image, GetPixelOutOfBounds)
{
    lkCommon::Image i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));

    lkCommon::Image::Pixel r1;
    EXPECT_FALSE(i.GetPixel(TEST_WIDTH, TEST_HEIGHT, r1));
    EXPECT_FALSE(i.GetPixel(TEST_WIDTH, TEST_HEIGHT, r1.r, r1.g, r1.b));
    EXPECT_FALSE(i.GetPixel(TEST_WIDTH, TEST_HEIGHT, r1.r, r1.g, r1.b, r1.a));
}
