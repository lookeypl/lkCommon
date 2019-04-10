#include <gtest/gtest.h>
#include <lkCommon/Utils/Image.hpp>

const uint32_t TEST_WIDTH = 10;
const uint32_t TEST_HEIGHT = 20;

const uint8_t TEST_PIXEL_RAW[] = {10, 30, 20, 14};
const uint8_t TEST_PIXEL_RAW_2[] = {20, 10, 18, 90};
const uint8_t TEST_PIXEL_RAW_BILINEAR_1_2[] = {15, 20, 19, 52};

const lkCommon::Utils::PixelUint4 TEST_PIXEL_ZERO;
const lkCommon::Utils::PixelUint4 TEST_PIXEL(TEST_PIXEL_RAW);
const lkCommon::Utils::PixelUint4 TEST_PIXEL_2(TEST_PIXEL_RAW_2);
const lkCommon::Utils::PixelUint4 TEST_PIXEL_BILINEAR_1_2(TEST_PIXEL_RAW_BILINEAR_1_2);

const uint32_t TEST_IMPORT_IMAGE_WIDTH = 5;
const uint32_t TEST_IMPORT_IMAGE_HEIGHT = 5;
const uint32_t TEST_IMPORT_IMAGE_SMALL_WIDTH = 3;
const uint32_t TEST_IMPORT_IMAGE_SMALL_HEIGHT = 3;

const std::vector<lkCommon::Utils::PixelUint4> TEST_IMPORT_IMAGE_5X5 {
    TEST_PIXEL,   TEST_PIXEL_2, TEST_PIXEL,   TEST_PIXEL_2, TEST_PIXEL,
    TEST_PIXEL_2, TEST_PIXEL,   TEST_PIXEL_2, TEST_PIXEL,   TEST_PIXEL_2,
    TEST_PIXEL,   TEST_PIXEL_2, TEST_PIXEL,   TEST_PIXEL_2, TEST_PIXEL,
    TEST_PIXEL_2, TEST_PIXEL,   TEST_PIXEL_2, TEST_PIXEL,   TEST_PIXEL_2,
    TEST_PIXEL,   TEST_PIXEL_2, TEST_PIXEL,   TEST_PIXEL_2, TEST_PIXEL,
};

const std::vector<lkCommon::Utils::PixelUint4> TEST_IMPORT_IMAGE_3X3 {
    TEST_PIXEL,   TEST_PIXEL_2, TEST_PIXEL,
    TEST_PIXEL_2, TEST_PIXEL,   TEST_PIXEL_2,
    TEST_PIXEL,   TEST_PIXEL_2, TEST_PIXEL,
};

const std::vector<lkCommon::Utils::PixelUint4> TEST_IMPORT_IMAGE_3X3_AS_5X5 {
    TEST_PIXEL,      TEST_PIXEL_2,    TEST_PIXEL,      TEST_PIXEL_ZERO, TEST_PIXEL_ZERO,
    TEST_PIXEL_2,    TEST_PIXEL,      TEST_PIXEL_2,    TEST_PIXEL_ZERO, TEST_PIXEL_ZERO,
    TEST_PIXEL,      TEST_PIXEL_2,    TEST_PIXEL,      TEST_PIXEL_ZERO, TEST_PIXEL_ZERO,
    TEST_PIXEL_ZERO, TEST_PIXEL_ZERO, TEST_PIXEL_ZERO, TEST_PIXEL_ZERO, TEST_PIXEL_ZERO,
    TEST_PIXEL_ZERO, TEST_PIXEL_ZERO, TEST_PIXEL_ZERO, TEST_PIXEL_ZERO, TEST_PIXEL_ZERO,
};


bool CompareRawPixels(const uint8_t p1[4], const uint8_t p2[4], bool withAlpha)
{
    return (p1[2] == p2[2]) && (p1[1] == p2[1]) && (p1[0] == p2[0]) && (withAlpha ? (p1[3] == p2[3]) : true);
}

TEST(Image, ConstructorDefault)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelFloat4> i;

    EXPECT_EQ(0, i.GetWidth());
    EXPECT_EQ(0, i.GetHeight());
}

TEST(Image, ConstructorSize)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelFloat4> i(TEST_WIDTH, TEST_HEIGHT);

    EXPECT_EQ(TEST_WIDTH, i.GetWidth());
    EXPECT_EQ(TEST_HEIGHT, i.GetHeight());
    EXPECT_NE(nullptr, i.GetDataPtr());
}

TEST(Image, ConstructorDataSameAsImage)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i(TEST_IMPORT_IMAGE_WIDTH, TEST_IMPORT_IMAGE_HEIGHT, 5, TEST_IMPORT_IMAGE_5X5);
    lkCommon::Utils::PixelUint4 p;

    ASSERT_EQ(TEST_IMPORT_IMAGE_WIDTH, i.GetWidth());
    ASSERT_EQ(TEST_IMPORT_IMAGE_HEIGHT, i.GetHeight());

    for (uint32_t y = 0; y < TEST_IMPORT_IMAGE_HEIGHT; ++y)
    {
        for (uint32_t x = 0; x < TEST_IMPORT_IMAGE_WIDTH; ++x)
        {
            i.GetPixel(x, y, p);
            EXPECT_EQ(TEST_IMPORT_IMAGE_5X5[x + (y * TEST_IMPORT_IMAGE_WIDTH)], p);
        }
    }
}

TEST(Image, ConstructorDataBiggerThanImage)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i(TEST_IMPORT_IMAGE_SMALL_WIDTH, TEST_IMPORT_IMAGE_SMALL_HEIGHT, 5, TEST_IMPORT_IMAGE_5X5);
    lkCommon::Utils::PixelUint4 p;

    ASSERT_EQ(TEST_IMPORT_IMAGE_SMALL_WIDTH, i.GetWidth());
    ASSERT_EQ(TEST_IMPORT_IMAGE_SMALL_HEIGHT, i.GetHeight());

    for (uint32_t y = 0; y < TEST_IMPORT_IMAGE_SMALL_HEIGHT; ++y)
    {
        for (uint32_t x = 0; x < TEST_IMPORT_IMAGE_SMALL_WIDTH; ++x)
        {
            i.GetPixel(x, y, p);
            EXPECT_EQ(TEST_IMPORT_IMAGE_3X3[x + (y * TEST_IMPORT_IMAGE_SMALL_WIDTH)], p);
        }
    }
}

TEST(Image, ConstructorDataSmallerThanImage)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i(TEST_IMPORT_IMAGE_WIDTH, TEST_IMPORT_IMAGE_HEIGHT, 3, TEST_IMPORT_IMAGE_3X3);
    lkCommon::Utils::PixelUint4 p;

    ASSERT_EQ(TEST_IMPORT_IMAGE_WIDTH, i.GetWidth());
    ASSERT_EQ(TEST_IMPORT_IMAGE_HEIGHT, i.GetHeight());

    for (uint32_t y = 0; y < TEST_IMPORT_IMAGE_HEIGHT; ++y)
    {
        for (uint32_t x = 0; x < TEST_IMPORT_IMAGE_WIDTH; ++x)
        {
            i.GetPixel(x, y, p);
            EXPECT_EQ(TEST_IMPORT_IMAGE_3X3_AS_5X5[x + (y * TEST_IMPORT_IMAGE_WIDTH)], p);
        }
    }
}

TEST(Image, Resize)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i;
    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
}

TEST(Image, ResizeToZero)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i(TEST_WIDTH, TEST_HEIGHT);
    EXPECT_FALSE(i.Resize(0, TEST_HEIGHT));
    EXPECT_FALSE(i.Resize(TEST_WIDTH, 0));
    EXPECT_FALSE(i.Resize(0, 0));
}

TEST(Image, GetDimensions)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i;
    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_EQ(i.GetWidth(), TEST_WIDTH);
    EXPECT_EQ(i.GetHeight(), TEST_HEIGHT);
}

TEST(Image, GetDataPointer)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i;

    EXPECT_EQ(i.GetDataPtr(), nullptr);
    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_NE(i.GetDataPtr(), nullptr);
}

TEST(Image, SetPixel)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL));
}

TEST(Image, SetPixelOutOfBounds)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_FALSE(i.SetPixel(TEST_WIDTH, 0, TEST_PIXEL));
    EXPECT_FALSE(i.SetPixel(0, TEST_HEIGHT, TEST_PIXEL));
}

TEST(Image, GetPixel)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));
    EXPECT_TRUE(i.SetPixel(0, 0, TEST_PIXEL));
    EXPECT_TRUE(i.SetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, TEST_PIXEL_2));

    lkCommon::Utils::PixelUint4 r1;
    lkCommon::Utils::PixelUint4 r2;
    EXPECT_TRUE(i.GetPixel(0, 0, r1));
    EXPECT_TRUE(i.GetPixel(TEST_WIDTH - 1, TEST_HEIGHT - 1, r2));

    EXPECT_EQ(r1, TEST_PIXEL);
    EXPECT_EQ(r2, TEST_PIXEL_2);
    EXPECT_NE(r1, r2);
}

TEST(Image, GetPixelOutOfBounds)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i;

    EXPECT_TRUE(i.Resize(TEST_WIDTH, TEST_HEIGHT));

    lkCommon::Utils::PixelUint4 r1;
    EXPECT_FALSE(i.GetPixel(TEST_WIDTH, TEST_HEIGHT, r1));
}

TEST(Image, SampleNearestUint4)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i(TEST_IMPORT_IMAGE_WIDTH, TEST_IMPORT_IMAGE_HEIGHT, 5, TEST_IMPORT_IMAGE_5X5);
    lkCommon::Utils::PixelUint4 p = i.Sample(0.5f, 0.5f, lkCommon::Utils::Sampling::NEAREST);

    EXPECT_EQ(TEST_PIXEL, p);
}

TEST(Image, SampleBilinearUint4)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i(TEST_IMPORT_IMAGE_WIDTH, TEST_IMPORT_IMAGE_HEIGHT, 5, TEST_IMPORT_IMAGE_5X5);
    lkCommon::Utils::PixelUint4 p = i.Sample(0.5f, 0.5f, lkCommon::Utils::Sampling::BILINEAR);

    EXPECT_EQ(TEST_PIXEL_BILINEAR_1_2, p);
}

TEST(Image, SampleFurthestCornerNearest)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i(TEST_IMPORT_IMAGE_WIDTH, TEST_IMPORT_IMAGE_HEIGHT, 5, TEST_IMPORT_IMAGE_5X5);
    lkCommon::Utils::PixelUint4 p = i.Sample(1.0f, 1.0f, lkCommon::Utils::Sampling::NEAREST);

    EXPECT_EQ(TEST_PIXEL, p);
}

TEST(Image, SampleFurthestCornerBilinear)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i(TEST_IMPORT_IMAGE_WIDTH, TEST_IMPORT_IMAGE_HEIGHT, 5, TEST_IMPORT_IMAGE_5X5);
    lkCommon::Utils::PixelUint4 p = i.Sample(1.0f, 1.0f, lkCommon::Utils::Sampling::BILINEAR);

    EXPECT_EQ(TEST_PIXEL, p);
}

TEST(Image, SampleClosestCornerNearest)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i(TEST_IMPORT_IMAGE_WIDTH, TEST_IMPORT_IMAGE_HEIGHT, 5, TEST_IMPORT_IMAGE_5X5);
    lkCommon::Utils::PixelUint4 p = i.Sample(0.0f, 0.0f, lkCommon::Utils::Sampling::NEAREST);

    EXPECT_EQ(TEST_PIXEL, p);
}

TEST(Image, SampleClosestCornerBilinear)
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelUint4> i(TEST_IMPORT_IMAGE_WIDTH, TEST_IMPORT_IMAGE_HEIGHT, 5, TEST_IMPORT_IMAGE_5X5);
    lkCommon::Utils::PixelUint4 p = i.Sample(0.0f, 0.0f, lkCommon::Utils::Sampling::BILINEAR);

    EXPECT_EQ(TEST_PIXEL, p);
}
