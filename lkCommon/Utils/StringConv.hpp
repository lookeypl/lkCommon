/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Module supporting conversion between Char Strings and Wide Char Strings
 *
 * @note There are differences in encoding for both supported platforms,
 * which are Windows and Linux - Windows systems use UTF-16 encoding for
 * non-ASCII sign support, while Linux based platforms use UTF-8. Additionally,
 * wchar_t type used by std::wstring takes 4 bytes on Linux, while on Windows it
 * is 2 byte large. To keep the encoding consistent, lkCommon library assumes
 * UTF-8 encoding everywhere and does the conversion only when necessary to
 * interact with Windows API. Below functions are meant to support this on
 * Windows and rarely should be used while interacting with lkCommon API, or
 * while implementing different lkCommon modules on Linux.
 */

#include <string>


namespace lkCommon {
namespace Utils {

/**
 * Converts Char String to Wide Char String
 *
 * @param[in]  in  Input char string
 * @param[out] out Output wide char string
 * @result True if conversion succeeds, false if there's an error. When false
 * is returned, contents of @p out are unspecified.
 */
bool StringToWString(const std::string& in, std::wstring& out);

/**
 * Converts Wide Char String to Char String
 *
 * @param[in]  in  Input wide char string.
 * @param[out] out Output char string.
 * @result True if conversion succeeds, false if there's an error. When false
 * is returned, contents of @p out are unspecified.
 */
bool WStringToString(const std::wstring& in, std::string& out);

} // namespace Utils
} // namespace lkCommon
