#include "lkCommon/System/Info.hpp"
#include "lkCommon/lkCommon.hpp"
#include <Windows.h>

namespace {

SYSTEM_INFO gSystemInfo;
bool gSystemInfoAcquired = false;

LKCOMMON_INLINE void FetchSystemInfo()
{
    if (!gSystemInfoAcquired)
    {
        GetSystemInfo(&gSystemInfo);
        gSystemInfoAcquired = true;
    }
}

} // namespace

namespace lkCommon {
namespace System {
namespace Info {


size_t GetCPUCount()
{
    return GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
}

size_t GetPageSize()
{
    FetchSystemInfo();
    return gSystemInfo.dwPageSize;
}


} // namespace Info
} // namespace System
} // namespace lkCommon
