#include "PCH.hpp"
#include "../Timer.hpp"


namespace lkCommon {

Timer::Timer()
{
    QueryPerformanceFrequency(&mFreq);
}

Timer::~Timer()
{
}

void Timer::Start()
{
    QueryPerformanceCounter(&mStart);
}

double Timer::Stop()
{
    LARGE_INTEGER stop;
    QueryPerformanceCounter(&stop);
    LONGLONG diff = stop.QuadPart - mStart.QuadPart;
    if (diff < 0)
        return 0.0;

    return static_cast<double>(diff) / static_cast<double>(mFreq.QuadPart);
}

} // namespace lkCommon
