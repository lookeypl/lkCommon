#pragma once

#ifndef _LKCOMMON_UTILS_OBJECTPOOL_HPP_
#error "Please include main header of Stack, not the implementation header."
#endif // _LKCOMMON_UTILS_OBJECTPOOL_HPP_


namespace lkCommon {
namespace Utils {

template <class T>
ObjectPool<T>::ObjectPool()
{
    AllocateNewChunk();
}

template <class T>
void ObjectPool<T>::AllocateNewChunk()
{
    mChunks.emplace();
    mUsedSlots.emplace(0);
}

template <class T>
T* ObjectPool<T>::Allocate()
{

}

template <class T>
void ObjectPool<T>::Free()
{
}

} // namespace Utils
} // namespace lkCommon
