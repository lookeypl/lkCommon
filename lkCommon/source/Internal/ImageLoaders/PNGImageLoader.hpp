#pragma once

#include "lkCommon/Utils/ImageLoader.hpp"

#include <png.h>
#include <cstdio>


namespace lkCommon {
namespace Utils {
namespace Internal {

class PNGImageLoader: public ImageLoader
{
    friend class ImageLoader;

    FILE* mPngFile;
    png_structp mPngReader;
    png_infop mPngInfo;

    void FillRowRGBAUchar(const png_bytep row, void* buf) const;
    void FillRowRGBAFloat(const png_bytep row, void* buf) const;

protected:
    bool SupportsFile(const std::string& path) override;

public:
    PNGImageLoader();
    ~PNGImageLoader();

    bool Load(const std::string& path) override;
    size_t FillData(void* buf, const size_t bufSize, const ImageFormat format) const override;
    void Release();
};

} // namespace Internal
} // namespace Utils
} // namespace lkCommon
