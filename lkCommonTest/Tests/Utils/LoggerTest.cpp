#include <gtest/gtest.h>
#include <lkCommon/Utils/Logger.hpp>

TEST(Logger, LogTest)
{
    LOGE("Error test log");
    LOGW("Warning test log");
    LOGI("Info test log");
    LOGD("Debug test log");
}
