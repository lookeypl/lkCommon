#include <gtest/gtest.h>
#include <lkCommon/System/Info.hpp>

TEST(Info, CPUCount)
{
    EXPECT_GT(lkCommon::System::Info::GetCPUCount(), 0u);
}

TEST(Info, PageSize)
{
    EXPECT_GT(lkCommon::System::Info::GetPageSize(), 0u);
}
