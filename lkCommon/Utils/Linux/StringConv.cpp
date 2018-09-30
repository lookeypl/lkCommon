#include "../StringConv.hpp"

#include "lkCommon.hpp"
#include "Utils/Logger.hpp"

#include <iconv.h>
#include <vector>


namespace lkCommon {
namespace Utils {

bool StringToWString(const std::string& in, std::wstring& out)
{
    // on Linux wchar_t is 32-bytes so wstrings are in UTF-32
    iconv_t converter = iconv_open("UTF-32LE", "UTF-8");
    if (converter == reinterpret_cast<iconv_t>(-1))
    {
        LOGE("Failed to initialize iconv context: " << strerror(errno));
        return false;
    }

    size_t increment = 8;
    size_t bufSize = in.size();

    while (true)
    {
        out.resize(bufSize);

        // have to do this cause iconv doesn't take const into account
        char* inBuf = const_cast<char*>(in.data());
        size_t inBufLen = in.size() * sizeof(char);
        char* outBuf = reinterpret_cast<char*>(const_cast<wchar_t*>(out.data()));
        size_t outBufLen = bufSize * sizeof(wchar_t);

        size_t result = iconv(converter, &inBuf, &inBufLen, &outBuf, &outBufLen);
        if (result == static_cast<size_t>(-1))
        {
            if (errno == E2BIG)
            {
                // increase buffer size and retry
                bufSize += increment;
                continue;
            }
            else
            {
                // sth else failed
                LOGE("Conversion error: " << strerror(errno));
                iconv_close(converter);
                return false;
            }
        }
        else
        {
            // trim our buffer and leave
            // outBufLen keeps leftover buffer size, so we must subtract it
            // from total buffer size and divide by wstring's value type
            out.resize(((bufSize * sizeof(wchar_t)) - outBufLen) / sizeof(wchar_t));
            break;
        }
    }

    iconv_close(converter);
    return true;
}

bool WStringToString(const std::wstring& in, std::string& out)
{
    // on Linux wchar_t is 32-bytes so wstrings are in UTF-32
    iconv_t converter = iconv_open("UTF-8", "UTF-32LE");
    if (converter == reinterpret_cast<iconv_t>(-1))
    {
        LOGE("Failed to initialize iconv context: " << strerror(errno));
        return false;
    }

    size_t increment = 8;
    size_t bufSize = in.size();

    while (true)
    {
        out.resize(bufSize);

        // have to do this cause iconv doesn't take const into account
        char* inBuf = reinterpret_cast<char*>(const_cast<wchar_t*>(in.data()));
        size_t inBufLen = in.size() * sizeof(wchar_t);
        char* outBuf = const_cast<char*>(out.data());
        size_t outBufLen = bufSize * sizeof(char);

        size_t result = iconv(converter, &inBuf, &inBufLen, &outBuf, &outBufLen);
        if (result == static_cast<size_t>(-1))
        {
            if (errno == E2BIG)
            {
                // increase buffer size and retry
                bufSize += increment;
                continue;
            }
            else
            {
                // sth else failed
                LOGE("Conversion error: " << strerror(errno));
                iconv_close(converter);
                return false;
            }
        }
        else
        {
            // trim our buffer and leave
            // outBufLen keeps leftover buffer size, so we must subtract it
            // from total buffer size and divide by wstring's value type
            out.resize(bufSize - outBufLen);
            break;
        }
    }

    iconv_close(converter);
    return true;
}

} // namespace Utils
} // namespace lkCommon
