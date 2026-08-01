#include "lkCommon/System/Library.hpp"

#include "lkCommon/lkCommon.hpp"
#include "lkCommon/Utils/Logger.hpp"
#include "lkCommon/Utils/StringConv.hpp"


namespace lkCommon {
namespace System {

Library::Library()
    : mModule(0)
{
}

Library::~Library()
{
    Close();
}

bool Library::Open(const std::string& path)
{
    if (mModule)
        return false;

    // append ".dll" only if needed
    std::string pathExt(path);
    std::string ext = ".dll";
    if (ext.compare(pathExt.substr(pathExt.size() - ext.size())) != 0)
        pathExt.append(ext);

    std::wstring widePath;
    if (!Utils::StringToWString(pathExt, widePath))
        return false;

    mModule = LoadLibrary(widePath.c_str());
    if (!mModule)
    {
        LOGE("Failed to load library " << path);
        return false;
    }

    return true;
}

void* Library::GetSymbol(const std::string& name)
{
    if (!mModule)
        return nullptr;

    FARPROC p = GetProcAddress(mModule, name.c_str());
    if (!p)
    {
        LOGE("Failed to acquire address for symbol " << name << ": " << GetLastError());
        return nullptr;
    }

    return static_cast<void*>(p);
}

void Library::Close()
{
    if (!mModule)
        return;

    FreeLibrary(mModule);
    mModule = nullptr;
}

} // namespace System
} // namespace lkCommon
