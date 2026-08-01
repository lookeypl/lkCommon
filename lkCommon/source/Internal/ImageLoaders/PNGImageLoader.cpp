#include "PNGImageLoader.hpp"

#include "lkCommon/Utils/Logger.hpp"

#include <png.h>
#include <fstream>
#include <csetjmp>
#include <cstdio>


namespace lkCommon {
namespace Utils {
namespace Internal {

PNGImageLoader::PNGImageLoader()
    : mPngFile(nullptr)
    , mPngReader(nullptr)
    , mPngInfo(nullptr)
{
}

PNGImageLoader::~PNGImageLoader()
{
    Release();
}

void PNGImageLoader::FillRowRGBAUchar(const png_bytep row, void* buf) const
{
    unsigned char* bufPtr = reinterpret_cast<unsigned char*>(buf);
    for (size_t i = 0; i < mWidth; ++i)
    {
        // automatically perform swap during read for Image class
        bufPtr[i * 4 + 0] = row[i * 4 + 2];
        bufPtr[i * 4 + 1] = row[i * 4 + 1];
        bufPtr[i * 4 + 2] = row[i * 4 + 0];
        bufPtr[i * 4 + 3] = row[i * 4 + 3];
    }
}

void PNGImageLoader::FillRowRGBAFloat(const png_bytep row, void* buf) const
{
    // pixels must be converted from 8-bit int to float
    float* bufPtr = reinterpret_cast<float*>(buf);
    for (size_t i = 0; i < mWidth; ++i)
    {
        // automatically perform swap during read for Image class
        bufPtr[i * 4 + 0] = static_cast<float>(row[i * 4 + 2]) / 255.0f;
        bufPtr[i * 4 + 1] = static_cast<float>(row[i * 4 + 1]) / 255.0f;
        bufPtr[i * 4 + 2] = static_cast<float>(row[i * 4 + 0]) / 255.0f;
        bufPtr[i * 4 + 3] = static_cast<float>(row[i * 4 + 3]) / 255.0f;
    }
}

bool PNGImageLoader::SupportsFile(const std::string& path)
{
    unsigned char intro[8];

    std::ifstream file(path, std::ifstream::binary);
    if (!file)
    {
        LOGE("Failed to open PNG file at path " << path);
        return false;
    }

    file.read(reinterpret_cast<char*>(intro), 8);

    if (png_sig_cmp(intro, 0, 8))
    {
        LOGE("Invalid PNG file provided: " << path);
        return false;
    }

    return true;
}

bool PNGImageLoader::Load(const std::string& path)
{
    if (!SupportsFile(path))
    {
        LOGE("Invalid file provided");
        return false;
    }

    mPngFile = fopen(path.c_str(), "rb");
    if (!mPngFile)
    {
        LOGE("Failed to open PNG file " << path);
        return false;
    }

    // TODO logging PNG thingies
    mPngReader = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!mPngReader)
    {
        LOGE("Failed to create PNG read struct");
        Release();
        return false;
    }

    mPngInfo = png_create_info_struct(mPngReader);
    if (!mPngInfo)
    {
        LOGE("Failed to create PNG info struct");
        Release();
        return false;
    }

    // sets jump location for libpng's longjmp() calls
    // in case something goes wrong, we will jump here with nonzero return and exit
    int jmpret = setjmp(png_jmpbuf(mPngReader));
    if (jmpret)
    {
        LOGE("Error while processing libpng calls for Load: " << jmpret);
        Release();
        return false;
    }

    png_init_io(mPngReader, mPngFile);

    png_read_info(mPngReader, mPngInfo);

    mWidth = png_get_image_width(mPngReader, mPngInfo);
    mHeight = png_get_image_height(mPngReader, mPngInfo);

    uint32_t colorType = png_get_color_type(mPngReader, mPngInfo);
    if (colorType != PNG_COLOR_TYPE_RGB && colorType != PNG_COLOR_TYPE_RGBA)
    {
        // TODO support those types
        LOGE("Unsupported PNG color type " << colorType);
        Release();
        return false;
    }

    uint32_t bitDepth = png_get_bit_depth(mPngReader, mPngInfo);

    if (bitDepth == 16)
        png_set_strip_16(mPngReader);

    if (colorType == PNG_COLOR_TYPE_RGB)
        png_set_filler(mPngReader, 0xFF, PNG_FILLER_AFTER);

    png_read_update_info(mPngReader, mPngInfo);

    return true;
}

size_t PNGImageLoader::FillData(void* buf, const size_t bufSize, const ImageFormat format) const
{
    if (!buf)
    {
        LOGE("Invalid buffer pointer");
        return 0;
    }

    if (format == ImageFormat::UNKNOWN)
    {
        LOGE("Invalid image format");
        return 0;
    }

    int jmpret = setjmp(png_jmpbuf(mPngReader));
    if (jmpret)
    {
        LOGE("Error while processing libpng calls for FillData: " << jmpret);
        return 0;
    }

    size_t rowSize = png_get_rowbytes(mPngReader, mPngInfo);
    if (bufSize < (rowSize * mHeight))
    {
        LOGE("Buffer not big enough");
        return 0;
    }

    size_t pixelSize = 0;
    switch (format)
    {
    case ImageFormat::RGBA_UCHAR: pixelSize = sizeof(unsigned char) * 4; break;
    case ImageFormat::RGBA_FLOAT: pixelSize = sizeof(float) * 4; break;
    default:
        LOGE("Unrecognized format " << static_cast<std::underlying_type<ImageFormat>::type>(format));
        return 0;
    };

    png_bytep rowBuffer = new png_byte[rowSize];
    unsigned char* bufPtr = reinterpret_cast<unsigned char*>(buf);
    size_t sizeRead = 0;
    for (uint32_t i = 0; i < mHeight; ++i)
    {
        png_read_row(mPngReader, rowBuffer, NULL);

        switch (format)
        {
        case ImageFormat::RGBA_UCHAR: FillRowRGBAUchar(rowBuffer, bufPtr); break;
        case ImageFormat::RGBA_FLOAT: FillRowRGBAFloat(rowBuffer, bufPtr); break;
        default:
            LOGE("Unrecognized format " << static_cast<std::underlying_type<ImageFormat>::type>(format));
            return 0;
        }

        sizeRead += rowSize;
        bufPtr += mWidth * pixelSize;
    }

    return sizeRead;
}

void PNGImageLoader::Release()
{
    if (mPngFile)
    {
        fclose(mPngFile);
        mPngFile = nullptr;
    }

    if (mPngReader)
    {
        if (mPngInfo)
        {
            png_destroy_info_struct(mPngReader, &mPngInfo);
            mPngInfo = nullptr;
        }

        png_destroy_read_struct(&mPngReader, nullptr, nullptr);
        mPngReader = nullptr;
    }
}

} // namespace Internal
} // namespace Utils
} // namespace lkCommon
