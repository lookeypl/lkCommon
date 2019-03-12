/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Main file for lkRay Tests
 */

#include <gtest/gtest.h>
#include <lkCommon/System/FS.hpp>
#include <lkCommon/Utils/Logger.hpp>

int main(int argc, char* argv[])
{
    std::string root;
#ifdef LKCOMMON_ROOT_DIR
    root = LKCOMMON_ROOT_DIR;
#endif // LKCOMMON_ROOT_DIR
    lkCommon::Utils::Logger::SetRootPathToStrip(root);

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

    printf("%s\n", root.c_str());
#if defined(WIN32) && defined(_CRTDBG_MAP_ALLOC)
    _CrtDumpMemoryLeaks();
#endif // defined(WIN32) && defined(_CRTDBG_MAP_ALLOC)

    return result;
}
