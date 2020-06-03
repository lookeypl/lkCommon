#pragma once

#ifndef _LKCOMMON_UTILS_STACK_HPP_
#error "Please include main header of Stack, not the implementation header."
#endif // _LKCOMMON_UTILS_STACK_HPP_

#include "Stack.hpp"


namespace lkCommon {
namespace Utils {

template <typename T, size_t N>
Stack<T, N>::Stack()
    : mStackPointer(0)
{
    mStack = new T[N];
}

template <typename T, size_t N>
Stack<T, N>::~Stack()
{
    delete[] mStack;
}

template <typename T, size_t N>
void Stack<T, N>::Push(const T& element)
{
    LKCOMMON_ASSERT(mStackPointer != N, "Stack not big enough, mStackPointer = " << mStackPointer << ", N = " << N);
    LKCOMMON_ASSERT(mStackPointer != UINT32_MAX, "Invalid stack pointer value.");

    mStack[mStackPointer] = element;
    mStackPointer++;
}

template <typename T, size_t N>
template <typename... Args>
void Stack<T, N>::Emplace(Args&&... args)
{
    LKCOMMON_ASSERT(mStackPointer != N, "Stack not big enough, mStackPointer = " << mStackPointer << ", N = " << N);
    LKCOMMON_ASSERT(mStackPointer != UINT32_MAX, "Invalid stack pointer value.");

    mStack[mStackPointer] = T(std::forward<Args>(args)...);
    mStackPointer++;
}

template <typename T, size_t N>
T Stack<T, N>::Pop()
{
    LKCOMMON_ASSERT(mStackPointer != 0, "Stack empty");

    mStackPointer--;
    return mStack[mStackPointer];
}

} // namespace Utils
} // namespace lkCommon
