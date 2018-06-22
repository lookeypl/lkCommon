#include "../UTF.hpp"

#include "lkCommon.hpp"
#include "Utils/Logger.hpp"


namespace lkCommon {
namespace Utils {

// This function should be unused under Linux
bool UTF8ToUTF16(const std::string& in, std::wstring& out)
{
    LKCOMMON_UNUSED(in); LKCOMMON_UNUSED(out);
    LOGE("Linux has no use for UTF-8 to UTF-16 converter. If you got here, you're doing something wrong.");
    return false;
}

bool UTF16ToUTF8(const std::wstring& in, std::string& out)
{
    LKCOMMON_UNUSED(in); LKCOMMON_UNUSED(out);
    LOGE("Linux has no use for UTF-16 to UTF-8 converter. If you got here, you're doing something wrong.");
    return false;
}

} // namespace Utils
} // namespace lkCommon
