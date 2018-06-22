#include <string>


namespace lkCommon {
namespace Utils {

bool UTF8ToUTF16(const std::string& in, std::wstring& out);
bool UTF16ToUTF8(const std::wstring& in, std::string& out);

} // namespace Utils
} // namespace lkCommon
