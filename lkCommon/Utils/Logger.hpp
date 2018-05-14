#pragma once

#include <sstream>

#ifdef ERROR
#undef ERROR
#endif

namespace lkCommon {
namespace Logger {

enum class LogLevel: unsigned char
{
    ERROR = 0,
    WARNING,
    INFO,
    DEBUG,
    MEMORY,
};

void Log(LogLevel level, const std::stringstream& msg);

} // namespace Logger
} // namespace lkCommon

#define LOG(level, msg) do { \
    std::stringstream ss; \
    ss << msg; \
    lkCommon::Logger::Log(level, ss); \
} while(0)

#ifdef _DEBUG
#define LOGD(msg) LOG(lkCommon::Logger::LogLevel::DEBUG, msg)
#else
#define LOGD(msg) do { } while(0)
#endif

#ifdef LKCOMMON_LOG_MEMORY
#define LOGM(msg) LOG(lkCommon::Logger::LogLevel::MEMORY, msg)
#else
#define LOGM(msg) do { } while(0)
#endif

#define LOGI(msg) LOG(lkCommon::Logger::LogLevel::INFO, msg)
#define LOGW(msg) LOG(lkCommon::Logger::LogLevel::WARNING, msg)
#define LOGE(msg) LOG(lkCommon::Logger::LogLevel::ERROR, msg)
