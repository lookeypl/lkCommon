#include "PCH.hpp"
#include "../FS.hpp"

#include "Common/Common.hpp"
#include "Common/Logger.hpp"

namespace lkCommon {
namespace FS {

bool CreateDir(const std::string& path)
{
    std::wstring pathWStr;
    UTF8ToUTF16(path, pathWStr);

    return CreateDirectory(pathWStr.c_str(), NULL);
}

bool Exists(const std::string& path)
{
    std::wstring pathWStr;
    UTF8ToUTF16(path, pathWStr);
    DWORD attribs = GetFileAttributes(pathWStr.c_str());

    return (attribs != INVALID_FILE_ATTRIBUTES);
}

std::string GetExecutablePath()
{
    HMODULE exe = GetModuleHandle(0);

    WCHAR path[512];
    GetModuleFileName(exe, path, 512);

    std::wstring pathWStr(path);
    std::string pathStr;
    lkCommon::UTF16ToUTF8(pathWStr, pathStr);

    std::string unifiedPathStr = UnifySlashes(pathStr);
    return unifiedPathStr;
}

std::string GetParentDir(const std::string& path)
{
    size_t trailingSlash = path.rfind('/');
    if (trailingSlash == std::string::npos)
        return path;

    return path.substr(0, trailingSlash);
}

std::string GetFileName(const std::string& path)
{
    size_t trailingSlash = path.rfind('/');
    if (trailingSlash == std::string::npos)
        return path;

    return path.substr(trailingSlash + 1);
}

uint64_t GetFileModificationTime(const std::string& path)
{
    FILETIME time;

    std::wstring pathWStr;
    if (!Common::UTF8ToUTF16(path, pathWStr))
        return 0;

    HANDLE file = CreateFile(pathWStr.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        DWORD err = GetLastError();
        LOGE("Failed to open file to get modification time: " << err);
        return 0;
    }

    bool getResult = GetFileTime(file, nullptr, nullptr, &time);
    CloseHandle(file);
    if (!getResult)
    {
        DWORD err = GetLastError();
        LOGE("Failed to get modification time of file: " << err);
        return 0;
    }

    return (static_cast<uint64_t>(time.dwHighDateTime) << 32) | time.dwLowDateTime;
}

std::string JoinPaths(const std::string& a, const std::string& b)
{
    std::string first, second;

    if (a.back() == '/')
        first = a.substr(0, a.size() - 2);
    else
        first = a;

    if (b.front() == '/')
        second = b.substr(1);
    else
        second = b;

    return first + '/' + second;
}

bool RemoveFile(const std::string& path)
{
    std::wstring pathWStr;
    if (!UTF8ToUTF16(path, pathWStr))
        return false;
    return DeleteFile(pathWStr.c_str());
}

bool SetCWD(const std::string& path)
{
    std::wstring wPath;
    lkCommon::UTF8ToUTF16(path, wPath);
    if (!SetCurrentDirectory(wPath.c_str()))
    {
        DWORD err = GetLastError();
        LOGE("Failed to set current working directory: " << err);
        return false;
    }

    WCHAR cwdBuf[512];
    GetCurrentDirectory(512, cwdBuf);

    std::wstring cwdWStr(cwdBuf);
    std::string cwdStr;
    UTF16ToUTF8(cwdWStr, cwdStr);

    LOGI("Set current working directory to: " << UnifySlashes(cwdStr));
    return true;
}

std::string UnifySlashes(const std::string& path)
{
    std::string result = path;

    for (char& c : result)
    {
        if (c == '\\')
            c = '/';
    }

    return result;
}

} // namespace FS
} // namespace lkCommon
