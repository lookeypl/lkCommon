/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Main library header containing commonly used defines throughout the
 * project.
 */

#pragma once

#include <cstring>
#include "Utils/Logger.hpp"

/**
 * Zeros memory of given structure/object/variable.
 */
#define LKCOMMON_ZERO_MEMORY(x) memset(&x, 0, sizeof(x))

/**
 * Suppresses unused variable warning
 */
#define LKCOMMON_UNUSED(x) (void)(x)

/**
 * @def LKCOMMON_INLINE
 *
 * Unified cross-platform macro forcing inlining of a function.
 *
 * Inline keyword does not guarantee inlining function with which it is used.
 * If there's such a need, this macro can be used to force inlining inside the
 * compiler.
 */

/**
 * @def LKCOMMON_ALIGN
 *
 * Aligns given class/struct to @p x bytes in memory.
 */

/**
 * @def LKCOMMON_RETURN_ALIGNED_MALLOC
 *
 * Calls aligned malloc and returns its value. Useful for new operator overloading.
 *
 * @todo this must be done differently (maybe new module Memutils.hpp?)
 */

/**
 * @def LKCOMMON_ALIGNED_FREE
 *
 * Calls aligned free in pair with aligned malloc above.
 * TODO this must be done differently (Memutils.hpp?)
 */
#ifdef WIN32

#define LKCOMMON_INLINE __forceinline
#define LKCOMMON_ALIGN(x) __declspec(align(x))
#define LKCOMMON_RETURN_ALIGNED_MALLOC(x, o) do { \
    return _aligned_malloc(x, o); \
} while(0)
#define LKCOMMON_ALIGNED_FREE(x) _aligned_free(x);

#elif defined(__linux__) || defined(__LINUX__)

#define LKCOMMON_INLINE inline __attribute__((always_inline))
#define LKCOMMON_ALIGN(x) __attribute__((aligned(x)))
#define LKCOMMON_RETURN_ALIGNED_MALLOC(x, o) do { \
    o = std::max(o, sizeof(void*)); \
    void* ptr = nullptr; \
    int ret = posix_memalign(&ptr, o, x); \
    if (ret != 0) \
        return nullptr; \
    return ptr; \
} while(0)
#define LKCOMMON_ALIGNED_FREE(x) free(x)

#else
#error "Target platform not supported"
#endif


/**
 * @def LKCOMMON_ASSERT
 *
 * Assertion macro. If expression exp is not true, logs error message and calls
 * assert(). Macro is active only when building with _DEBUG macro predefined.
 */
#ifdef _DEBUG

#include <cassert>

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
