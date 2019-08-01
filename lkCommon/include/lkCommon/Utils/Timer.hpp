#pragma once

#ifdef WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif // NOMINMAX

    #include <Windows.h>
#elif defined(__linux__) | defined(__LINUX__)
    #include <time.h>
#endif


namespace lkCommon {
namespace Utils {

class Timer
{
    double mStartPoint;

#ifdef WIN32
    LARGE_INTEGER mFreq;
#elif defined(__linux__) | defined(__LINUX__)
#else
#error "Target platform not supported."
#endif

public:
    /**
     * Default constructor of Timer class.
     */
    Timer();
    ~Timer() = default;

    /**
     * Starts time measurement, setting current time as start point.
     */
    void Start();

    /**
     * Starts time measurement, offsetting the start point by given value.
     *
     * @p[in] startOffset Offset of time in seconds.
     *
     * This call allows to start counting time from already known point in time (ex. from 1 second
     * backwards from current moment. So following sequence of calls:
     *
     * @code{.cpp}
     * Timer t;
     * t.Start(1.0);
     * // ...wait 1 second...
     * double delta = t.Stop();
     * @endcode
     *
     * will set delta variable to 2.0 instead of 1.0, despite waiting for 1 second.
     */
    void Start(double startOffset);

    /**
     * Stop the measurement and get time elapsed from last Start() call in seconds.
     *
     * @return Time elapsed since last Start() call.
     *
     * @note This call does NOT reset start point.
     */
    double Stop();
};

} // namespace Utils
} // namespace lkCommon
