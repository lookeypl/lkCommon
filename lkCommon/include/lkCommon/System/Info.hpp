#pragma once

#include <cstdlib>


namespace lkCommon {
namespace System {
namespace Info {


// Acquires number of logical processors in the system
size_t GetCPUCount();

// Acquires size of single page in bytes
size_t GetPageSize();


} // namespace Info
} // namespace System
} // namespace lkCommon
