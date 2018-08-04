#include <gtest/gtest.h>
#include <lkCommon/System/Info.hpp>

TEST(Info, CPUCount)
{
    std::cout << lkCommon::System::Info::GetCPUCount() << std::endl;
    EXPECT_GT(lkCommon::System::Info::GetCPUCount(), 0);
}
