#include "lkCommon/Utils/Timer.hpp"

#include "lkCommon/Utils/Logger.hpp"


namespace lkCommon {
namespace Utils {

Timer::Timer()
{
}

void Timer::Start()
{
    timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    mStartPoint = (start.tv_sec) + (start.tv_nsec) * 0.000'000'001;
}

void Timer::Start(double startOffset)
{
    Start();
    mStartPoint -= startOffset;
}

double Timer::Stop()
{
    timespec stop;
    clock_gettime(CLOCK_MONOTONIC, &stop);

    double stopPoint = (stop.tv_sec) + (stop.tv_nsec) * 0.000'000'001;
    return stopPoint - mStartPoint;
}

} // namespace Utils
} // namespace lkCommon
