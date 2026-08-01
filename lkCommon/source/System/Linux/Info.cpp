#include "lkCommon/System/Info.hpp"
#include <sys/sysinfo.h>
#include <unistd.h>

namespace lkCommon {
namespace System {
namespace Info {


size_t GetCPUCount()
{
    return get_nprocs();
}

size_t GetPageSize()
{
    return static_cast<size_t>(getpagesize());
}


} // namespace Info
} // namespace System
} // namespace lkCommon
