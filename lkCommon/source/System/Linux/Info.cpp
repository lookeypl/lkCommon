#include "lkCommon/System/Info.hpp"
#include <sys/sysinfo.h>

namespace lkCommon {
namespace System {
namespace Info {


// Acquires number of logical processors in the system
size_t GetCPUCount()
{
    return get_nprocs();
}


} // namespace Info
} // namespace System
} // namespace lkCommon
