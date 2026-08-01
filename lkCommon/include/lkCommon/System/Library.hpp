#pragma once

#ifdef WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif // NOMINMAX

    #include <Windows.h>
#endif // WIN32

#include <string>


namespace lkCommon {
namespace System {

class Library
{
#ifdef WIN32
    HMODULE mModule;
#elif defined(__linux__) | defined(__LINUX__)
    void* mModule;
#else
#error "Target platform not supported."
#endif


public:
    Library();
    ~Library();

    /**
     * Opens a library for further symbol loading.
     *
     * libname should be just a library name - any additional prefixs/suffixes
     * will get added automatically depending on the platform.
     */
    bool Open(const std::string& libname);
    void* GetSymbol(const std::string& name);
    void Close();
};

} // namespace System
} // namespace lkCommon
