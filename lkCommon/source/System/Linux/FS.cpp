#include "lkCommon/System/FS.hpp"

#include "lkCommon/lkCommon.hpp"
#include "lkCommon/Utils/Logger.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


namespace {

const std::string PROC_SELF_EXE_PATH = "/proc/self/exe";
const uint32_t CHAR_BUFFER_SIZE = 512;

} // namespace


namespace lkCommon {
namespace System {
namespace FS {

bool CreateDir(const std::string& path)
{
    return (mkdir(path.c_str(), 0777) >= 0);
}

bool Exists(const std::string& path)
{
    return (access(path.c_str(), F_OK) >= 0);
}

std::string GetExecutablePath()
{
    char buffer[CHAR_BUFFER_SIZE];
    ssize_t ret = readlink(PROC_SELF_EXE_PATH.c_str(), buffer, CHAR_BUFFER_SIZE);
    if (ret < 0)
    {
        LOGE("Failed to read " << PROC_SELF_EXE_PATH << ": " <<
                errno << " (" << strerror(errno) << ")");
        return std::string();
    }

    return std::string(buffer);
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
    struct ::stat attributes;
    if (::stat(path.c_str(), &attributes) < 0)
    {
        LOGE("Failed to stat " << path << ": " <<
             errno << " (" << strerror(errno) << ")");
        return 0;
    }

    return attributes.st_mtim.tv_nsec +
           (attributes.st_mtim.tv_sec * 1'000'000'000);
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
    if (unlink(path.c_str()) < 0)
    {
        LOGE("Failed to unlink file " << path << ": " << errno <<
             " (" << strerror(errno) << ")");
        return false;
    }

    LOGD("Removed file " << path);
    return true;
}

bool SetCWD(const std::string& path)
{
    if (chdir(path.c_str()) < 0)
    {
        LOGE("Failed to set current directory to " << path << ": " << errno <<
             " (" << strerror(errno) << ")");
        return false;
    }

    LOGD("Set current working directory to: " << path);
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
} // namespace System
} // namespace lkCommon
