#include <gtest/gtest.h>
#include <lkCommon/Math/RingAverage.hpp>
#include <array>
#include <algorithm>
#include <numeric>

using namespace lkCommon::Math;

const size_t RING_AVERAGE_TEST_COUNT = 100;

template <typename T, size_t N>
T RingAverageTestResult(T start)
{
    std::vector<T> v(N);
    std::iota(v.begin(), v.end(), start);
    T sum = std::accumulate(v.begin(), v.end(), static_cast<T>(0));
    return sum / static_cast<T>(N);
}

template <typename T, size_t N>
void TestRingAverage()
{
    RingAverage<T, N> avg;

    size_t i = 0;
    for (; i < N; ++i)
        avg.Add(static_cast<T>(i));

    EXPECT_EQ(avg.Get(), (RingAverageTestResult<T, N>(0)));

    // add another value to the test, this should circulate the buffer
    // replacing one of the starting values and raising the average
    avg.Add(static_cast<T>(i));
    EXPECT_EQ(avg.Get(), (RingAverageTestResult<T, N>(1)));
}

TEST(RingAverage, Int)
{
    TestRingAverage<int, RING_AVERAGE_TEST_COUNT>();
}

TEST(RingAverage, UnsignedInt)
{
    TestRingAverage<unsigned int, RING_AVERAGE_TEST_COUNT>();
}

TEST(RingAverage, Float)
{
    TestRingAverage<float, RING_AVERAGE_TEST_COUNT>();
}

TEST(RingAverage, Double)
{
    TestRingAverage<double, RING_AVERAGE_TEST_COUNT>();
}
