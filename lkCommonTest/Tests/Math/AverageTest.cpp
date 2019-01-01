#include <gtest/gtest.h>
#include <lkCommon/Math/Average.hpp>
#include <array>
#include <algorithm>
#include <numeric>

using namespace lkCommon::Math;

const size_t AVERAGE_TEST_COUNT = 100;

template <typename T, size_t N>
T AverageTestResult(T start)
{
    std::vector<T> v(N);
    std::iota(v.begin(), v.end(), start);
    T sum = std::accumulate(v.begin(), v.end(), static_cast<T>(0));
    return sum / static_cast<T>(N);
}

template <typename T, size_t N>
void TestAverage()
{
    Average<T> avg;

    size_t i = 0;
    for (; i < N; ++i)
        avg.Add(static_cast<T>(i));

    EXPECT_EQ(avg.Get(), (AverageTestResult<T, N>(0)));
}

TEST(Average, Int)
{
    TestAverage<int, AVERAGE_TEST_COUNT>();
}

TEST(Average, UnsignedInt)
{
    TestAverage<unsigned int, AVERAGE_TEST_COUNT>();
}

TEST(Average, Float)
{
    TestAverage<float, AVERAGE_TEST_COUNT>();
}

TEST(Average, Double)
{
    TestAverage<double, AVERAGE_TEST_COUNT>();
}
