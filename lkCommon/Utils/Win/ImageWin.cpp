#include "../Image.hpp"


/**
 * Below implementations are empty placeholders to prevent linker errors
 *
 * Windows implementation does not require platform-specific elements to
 * speed up displaying on Window
 */

namespace lkCommon {
namespace Utils {

void Image::InitPlatformSpecific()
{
}

bool Image::ResizePlatformSpecific()
{
    return true;
}

void Image::ReleasePlatformSpecific()
{
}

void* Image::GetPlatformImageHandle() const
{
    LOGE("Windows implementation of Image does not contain platform-specific code");
    return nullptr;
}

} // namespace Utils
} // namespace lkCommon
