#pragma once

#ifndef _LKCOMMON_UTILS_SORT_HPP_
#error "Please include main header of Sort, not the implementation header."
#endif // _LKCOMMON_UTILS_SORT_HPP_


namespace lkCommon {
namespace Utils {
namespace Impl {

template <typename T>
size_t QuickSortPartition(std::vector<T>& arr, size_t low, size_t high)
{
    T pivot = arr[low];
    size_t i = low;
    size_t j = high;

    while (true)
    {
        while (arr[i] < pivot)
            i++;

        while (arr[j] > pivot)
            j--;

        if (i >= j)
            return j;

        // swap
        T temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

template <typename T>
void QuickSortInternal(std::vector<T>& arr, size_t low, size_t high)
{
    if (low < high)
    {
        size_t pivot = QuickSortPartition(arr, low, high);
        QuickSortInternal(arr, low, pivot);
        QuickSortInternal(arr, pivot+1, high);
    }
}

// Old naive implementation, done just by reading the algorithm
template <typename T>
void BitonicSortBoxOld(std::vector<T>& arr, uint32_t boxSize, uint32_t boxOffset, bool greaterComp)
{
    for (uint32_t pass = boxSize; pass > 1; pass >>=1)
    {
        for (uint32_t j = 0; j < boxSize; j += pass)
        {
            uint32_t hp = pass >> 1;
            for (uint32_t i = 0; i < hp; ++i)
            {
                uint32_t a = boxOffset + j + i;
                uint32_t b = boxOffset + j + i + hp;
                if ((arr[a] < arr[b]) == greaterComp)
                {
                    T temp = arr[a];
                    arr[a] = arr[b];
                    arr[b] = temp;
                }
            }
        }
    }
}

// Part of the code, which could be easily invoked in a compute shader
template <typename T>
void BitonicSortBox(std::vector<T>& arr, uint32_t invocationID, uint32_t boxSize, uint32_t compareDist, bool greaterComp)
{
    uint32_t size = static_cast<uint32_t>(arr.size());

    for (uint32_t n = invocationID; n < size; n += 2)
    {
        uint32_t x = n ^ compareDist;

        if (x > n)
        {
            if (n & boxSize)
            {
                if ((arr[n] > arr[x]) == greaterComp)
                {
                    T temp = arr[n];
                    arr[n] = arr[x];
                    arr[x] = temp;
                }
            }
            else
            {
                if ((arr[n] < arr[x]) == greaterComp)
                {
                    T temp = arr[n];
                    arr[n] = arr[x];
                    arr[x] = temp;
                }
            }
        }
    }
}

// Below implementation was done so it could be easily ported for a compute shader code
template <typename T>
void BitonicSortInternal(std::vector<T>& arr, bool ascending)
{
    // TODO multithread the approach
    uint32_t size = static_cast<uint32_t>(arr.size());

    for (uint32_t boxSize = 2; boxSize <= size; boxSize <<= 1)
    {
        for (uint32_t compareDist = (boxSize >> 1); compareDist > 0; compareDist >>= 1)
        {
            for (uint32_t invocationID = 0; invocationID < 2; ++invocationID)
                BitonicSortBox(arr, invocationID, boxSize, compareDist, !ascending);
        }
    }
}

} // namespace Impl
} // namespace Utils
} // namespace lkCommon
