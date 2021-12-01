#include <gtest/gtest.h>
#include <lkCommon/System/FS.hpp>
#include <lkCommon/Utils/Logger.hpp>

#ifdef WIN32
#define LKCOMMON_ROOT_PATH_REL_TO_EXE "../../.."
#elif defined(__linux__) || defined(__LINUX__)
#define LKCOMMON_ROOT_PATH_REL_TO_EXE "../.."
#else
#error "Platform not supported"
#endif

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
                LKCOMMON_ROOT_PATH_REL_TO_EXE
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
