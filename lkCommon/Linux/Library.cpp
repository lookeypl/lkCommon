#include "PCH.hpp"
#include "../Library.hpp"
#include "../lkCommon.hpp"
#include "../Logger.hpp"

#include <dlfcn.h>


namespace lkCommon {

Library::Library()
    : mModule(nullptr)
{
}

Library::~Library()
{
    Close();
}

bool Library::Open(const std::string& libname)
{
    Close();

    std::string file = "lib";
    file += libname + ".so";

    mModule = dlopen(file.c_str(), RTLD_LAZY);
    if (mModule == nullptr)
    {
        LOGE("Failed to load library " << file << ": " << dlerror());
        return false;
    }

    return true;
}

void* Library::GetSymbol(const std::string& name)
{
    if (mModule == nullptr)
        return nullptr;

    // clearing dlerror first is required before symming because dlerror informs
    // us about any dlsym errors, and nullptr is a valid value.
    dlerror();

    void* fptr = dlsym(mModule, name.c_str());
    char* err = dlerror();
    if (err != nullptr)
    {
        LOGE("Failed to extract pointer to symbol " << name << ": " << err);
        return nullptr;
    }

    return fptr;
}

void Library::Close()
{
    if (mModule == nullptr)
        return;

    if (dlclose(mModule))
        LOGE("Failure while closing library: " << dlerror());

    mModule = nullptr;
}

} // namespace lkCommon
