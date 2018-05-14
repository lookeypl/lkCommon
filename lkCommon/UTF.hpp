#include <string>


namespace lkCommon {

bool UTF8ToUTF16(const std::string& in, std::wstring& out);
bool UTF16ToUTF8(const std::wstring& in, std::string& out);

} // namespace lkCommon