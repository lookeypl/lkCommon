#include "lkCommon/Utils/ImageLoader.hpp"

#include "lkCommon/lkCommon.hpp"
#include "ImageLoaders/PNGImageLoader.hpp"


namespace lkCommon {
namespace Utils {

ImageLoader::ImageLoader()
    : mWidth(0)
    , mHeight(0)
{
}

std::unique_ptr<ImageLoader> ImageLoader::SelectLoader(const std::string& path)
{
    LKCOMMON_UNUSED(path);

    return std::make_unique<Internal::PNGImageLoader>();
}

} // namespace Utils
} // namespace lkCommon
