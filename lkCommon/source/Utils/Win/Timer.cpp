#include "lkCommon/Utils/Timer.hpp"


namespace lkCommon {
namespace Utils {

Timer::Timer()
{
    QueryPerformanceFrequency(&mFreq);
}

void Timer::Start()
{
    LARGE_INTEGER start;
    QueryPerformanceCounter(&start);
    mStartPoint = static_cast<double>(start.QuadPart) / static_cast<double>(mFreq.QuadPart);
}

void Timer::Start(double startOffset)
{
    Start();
    mStartPoint -= startOffset;
}

double Timer::Stop()
{
    LARGE_INTEGER stop;
    QueryPerformanceCounter(&stop);
    double stopPoint = static_cast<double>(stop.QuadPart) / static_cast<double>(mFreq.QuadPart);
    return stopPoint - mStartPoint;
}

} // namespace Utils
} // namespace lkCommon
