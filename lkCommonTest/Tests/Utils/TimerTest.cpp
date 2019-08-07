#include <gtest/gtest.h>
#include <lkCommon/Utils/Timer.hpp>

#include <thread>


const double START_STOP_EPSILON = 0.001;
const double START_STOP_EXPECTED_DELTA = 0.3;
const double START_OFFSET = 0.5;


TEST(Timer, Constructor)
{
    lkCommon::Utils::Timer t;
}

TEST(Timer, StartStop)
{
    lkCommon::Utils::Timer t;

    t.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    double delta = t.Stop();

    EXPECT_NEAR(START_STOP_EXPECTED_DELTA, delta, START_STOP_EPSILON);
}

TEST(Timer, StartOffsetStop)
{
    lkCommon::Utils::Timer t;

    t.Start(START_OFFSET);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    double delta = t.Stop();

    EXPECT_NEAR(START_STOP_EXPECTED_DELTA + START_OFFSET, delta, START_STOP_EPSILON);
}
