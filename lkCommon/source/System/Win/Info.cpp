#include "lkCommon/System/Info.hpp"
#include <Windows.h>

namespace lkCommon {
namespace System {
namespace Info {

size_t GetCPUCount()
{
    return GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
}

} // namespace Info
} // namespace System
} // namespace lkCommon
