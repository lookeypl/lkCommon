#include "PCH.hpp"
#include "../Timer.hpp"

#include "Utils/Logger.hpp"


namespace lkCommon {

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::Start()
{
    clock_gettime(CLOCK_MONOTONIC, &mStart);
}

double Timer::Stop()
{
    timespec stop;
    clock_gettime(CLOCK_MONOTONIC, &stop);

    double delta = (stop.tv_sec - mStart.tv_sec) +
                   (stop.tv_nsec - mStart.tv_nsec) * 0.000'000'001;

    mStart.tv_sec = stop.tv_sec;
    mStart.tv_nsec = stop.tv_nsec;
    return delta;
}

} // namespace lkCommon
