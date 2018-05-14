#pragma once

#include <cstring>
#include "Utils/Logger.hpp"

// Zeros memory of given structure/object/variable
#define LKCOMMON_ZERO_MEMORY(x) memset(&x, 0, sizeof(x))

// Suppresses unused variable warning
#define LKCOMMON_UNUSED(x) (void)(x)


#ifdef WIN32

// Forces inlining of given function
#define LKCOMMON_INLINE __forceinline

// Aligns given class/struct to x bytes in memory
#define LKCOMMON_ALIGN(x) __declspec(align(x))

// Calls aligned malloc and returns its value. Useful for new operator overloading.
// TODO this must be done differently (Memutils.hpp?)
#define LKCOMMON_RETURN_ALIGNED_MALLOC(x, o) do { \
    return _aligned_malloc(x, o); \
} while(0)

// Calls aligned free in pair with aligned malloc above.
// TODO this must be done differently (Memutils.hpp?)
#define LKCOMMON_ALIGNED_FREE(x) _aligned_free(x);

#elif defined(__linux__) || defined(__LINUX__)

// Forces inlining of given function
#define LKCOMMON_INLINE __attribute__((always_inline))

// Aligns given class/struct to x bytes in memory
#define LKCOMMON_ALIGN(x) __attribute__((aligned(x)))

// Calls aligned malloc and returns its value. Useful for new operator overloading.
// TODO this must be done differently (Memutils.hpp?)
#define LKCOMMON_RETURN_ALIGNED_MALLOC(x, o) do { \
    o = std::max(o, sizeof(void*)); \
    void* ptr = nullptr; \
    int ret = posix_memalign(&ptr, o, x); \
    if (ret != 0) \
        return nullptr; \
    return ptr; \
} while(0)

// Calls aligned free in pair with aligned malloc above.
// TODO this must be done differently (Memutils.hpp?)
#define LKCOMMON_ALIGNED_FREE(x) free(x)

#else
#error "Target platform not supported"
#endif


#ifdef _DEBUG

// Assertion macro. If expression exp is not true, logs error message and calls assert()
// Macro is active only when building with _DEBUG macro predefined
#define LKCOMMON_ASSERT(exp, msg) do \
{ \
    if (!(exp)) \
    { \
        LOGE("Assertion " << #exp << " failed: " << msg); \
        assert(exp); \
    } \
} while(0)

#else // _DEBUG
#define LKCOMMON_ASSERT(exp, msg) do { } while(0)
#endif // _DEBUG
