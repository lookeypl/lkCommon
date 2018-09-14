/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Main file for lkRay Tests
 */

#include <gtest/gtest.h>
#include <lkCommon/System/FS.hpp>

int main(int argc, char* argv[])
{
    if (!lkCommon::System::FS::SetCWD(
            lkCommon::System::FS::JoinPaths(
                lkCommon::System::FS::GetParentDir(lkCommon::System::FS::GetExecutablePath()),
                "../../.."
                )
            )
        )
    {
        return -1;
    }

    testing::InitGoogleTest(&argc, argv);

    int result = RUN_ALL_TESTS();

#if defined(WIN32) && defined(_CRTDBG_MAP_ALLOC)
    _CrtDumpMemoryLeaks();
#endif // defined(WIN32) && defined(_CRTDBG_MAP_ALLOC)

    return result;
}
