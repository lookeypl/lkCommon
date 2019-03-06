#pragma once

#ifndef _LKCOMMON_UTILS_STATIC_STACK_H_
#error "Please include main header of StaticStack, not the implementation header."
#endif // _LKCOMMON_UTILS_STATIC_STACK_H_

#include "StaticStack.hpp"


namespace lkCommon {
namespace Utils {

template <typename T, size_t N>
StaticStack<T, N>::StaticStack()
    : mStackPointer(0)
{
}

template <typename T, size_t N>
void StaticStack<T, N>::Push(const T& element)
{
    LKCOMMON_ASSERT(mStackPointer != N, "Stack not big enough, mStackPointer = " << mStackPointer << ", N = " << N);
    LKCOMMON_ASSERT(mStackPointer != UINT32_MAX, "Invalid stack pointer value.");

    mStack[mStackPointer] = element;
    mStackPointer++;
}

template <typename T, size_t N>
template <typename... Args>
void StaticStack<T, N>::Emplace(Args&&... args)
{
    LKCOMMON_ASSERT(mStackPointer != N, "Stack not big enough, mStackPointer = " << mStackPointer << ", N = " << N);
    LKCOMMON_ASSERT(mStackPointer != UINT32_MAX, "Invalid stack pointer value.");

    mStack[mStackPointer] = T(std::forward<Args>(args)...);
    mStackPointer++;
}

template <typename T, size_t N>
T StaticStack<T, N>::Pop()
{
    LKCOMMON_ASSERT(mStackPointer != 0, "Stack empty");

    mStackPointer--;
    return mStack[mStackPointer];
}

} // namespace Utils
} // namespace lkCommon
