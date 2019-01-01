#pragma once

#include <mutex>


namespace lkCommon {
namespace Math {

/**
 * Thread-safe object calculating an average of given numbers
 */
template <typename V>
class Average
{
    V mSum;
    uint32_t mValueCount;
    mutable std::mutex mMutex;

public:
    Average();

    V Get() const;
    void Add(V value);
    void Clear();
};

} // namespace Math
} // namespace lkCommon

#include "AverageImpl.hpp"
