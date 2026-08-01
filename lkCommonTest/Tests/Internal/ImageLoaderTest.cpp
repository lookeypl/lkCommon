#include <gtest/gtest.h>
#include <lkCommon/Utils/ImageLoader.hpp>

#include <string>
#include <vector>

using namespace lkCommon::Utils;

const std::string TEST_IMAGE_PATH_PNG = "Data/PNG/test_image_5x5.png";



template<typename T> struct ExpectedTestData;

template<> struct ExpectedTestData<unsigned char>
{
    static constexpr ImageFormat format = ImageFormat::RGBA_UCHAR;
    static constexpr unsigned char pixels[] = {
        // pixel components are expected to be swapped, for Image implementation sake
        20, 30, 10, 14,
        18, 10, 20, 90,
    };
};

template<> struct ExpectedTestData<float>
{
    static constexpr ImageFormat format = ImageFormat::RGBA_FLOAT;
    static constexpr float pixels[8] = {
        // pixel components are expected to be swapped, for Image implementation sake
        20.0f / 255.0f, 30.0f / 255.0f, 10.0f / 255.0f, 14.0f / 255.0f,
        18.0f / 255.0f, 10.0f / 255.0f, 20.0f / 255.0f, 90.0f / 255.0f,
    };
};

constexpr unsigned char ExpectedTestData<unsigned char>::pixels[];
constexpr float ExpectedTestData<float>::pixels[];



template <typename BufferDataType>
void Test_LoadPNG_FillData()
{
    ImageLoaderPtr l = ImageLoader::SelectLoader(TEST_IMAGE_PATH_PNG);
    ASSERT_NE(nullptr, l.get());

    ASSERT_TRUE(l->Load(TEST_IMAGE_PATH_PNG));

    std::vector<BufferDataType> buffer(l->GetWidth() * l->GetHeight() * sizeof(BufferDataType) * 4);
    ASSERT_TRUE(l->FillData(buffer.data(), buffer.size(), ExpectedTestData<BufferDataType>::format));

    for (uint32_t i = 0; i < (l->GetWidth() * l->GetHeight()); ++i)
    {
        EXPECT_EQ(ExpectedTestData<BufferDataType>::pixels[i % 8], buffer[i]);
    }
}

TEST(ImageLoader, SelectLoaderPNG)
{
    ImageLoaderPtr l = ImageLoader::SelectLoader(TEST_IMAGE_PATH_PNG);
    ASSERT_NE(nullptr, l.get());
}

TEST(ImageLoader, LoadPNG_FillDataUChar)
{
    Test_LoadPNG_FillData<unsigned char>();
}

TEST(ImageLoader, LoadPNG_FillDataFloat)
{
    Test_LoadPNG_FillData<float>();
}
