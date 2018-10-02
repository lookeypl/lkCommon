/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Logger module implementation
 */

#pragma once

#include <sstream>


namespace lkCommon {
namespace Utils {

/**
 * Namespace keeping logging-related utilities.
 *
 * Available Log levels:
 * \li ERROR - indicates an error message, colored in bright red.
 * \li WARNING - indicates a warning message, colored in bright yellow.
 * \li INFO - indicates a regular message, displayed without color enhancements.
 * \li DEBUG - indicates a debug message, colored in light blue. Debug logs are
 * disabled when building in Release mode (without _DEBUG define)
 * \li MEMORY - indicates a memory-related message (ex. marking allocation of
 * resources). This is useful when user wants to keep track of all resources
 * which are allocated by the program. MEMORY logs are off by default - to
 * enable them, define LKCOMMON_LOG_MEMORY preprocessor macro.
 *
 * Logger module writes messages to following predefined backends:
 * \li stdout - standard console output.
 * \li log.txt - file created on first use of Log function.
 * \li VS Console (Windows only) - Debug console in Visual Studio.
 *
 * @note Functions in scope of this namespace should NOT be used. Instead,
 * there are numerous LOG* defines which allow to directly print logs.
 *
 * @remarks Ideas/tasks to do with Logger:
 *   * Evolve Logger backends to be more sophisticated (ex. to allow in
 *   * File backend should create the file on start, not on first Log() call
 */
namespace Logger {

/**
 * Log levels which are used when logging a message.
 *
 * Available log levels are described in-depth in /namespace Logger namespace
 * description.
 *
 * @note These enums should not be used directly. Instead, one of LOG* macros
 * should be used to print a message using Logger module.
 */
enum class LogLevel: unsigned char
{
    ERROR = 0,
    WARNING,
    INFO,
    DEBUG,
    MEMORY,
};

/**
 * Writes a @p msg string stream to predefined Log outputs at @p level level.
 *
 * Available logging backends are described in-depth in /namespace Logger
 * namespace description.
 *
 * @note This function should not be used directly. Instead, one of LOG* macros
 * should be used to print a message using Logger module.
 */
void Log(LogLevel level, const std::stringstream& msg);

} // namespace Logger
} // namespace Utils
} // namespace lkCommon

#define LOG(level, msg) do { \
    std::stringstream ss; \
    ss << msg; \
    lkCommon::Utils::Logger::Log(level, ss); \
} while(0)

/**
 * @def LOGD
 *
 * Macro used for logging Debug messages. Available only in Debug builds
 * (when _DEBUG is defined).
 */
#ifdef _DEBUG
#define LOGD(msg) LOG(lkCommon::Utils::Logger::LogLevel::DEBUG, msg)
#else
#define LOGD(msg) do { } while(0)
#endif

/**
 * @def LOGM
 *
 * Macro used for logging Memory-related messages. Available only when
 * LKCOMMON_LOG_MEMORY macro is defined.
 */
#ifdef LKCOMMON_LOG_MEMORY
#define LOGM(msg) LOG(lkCommon::Utils::Logger::LogLevel::MEMORY, msg)
#else
#define LOGM(msg) do { } while(0)
#endif

/**
 * Macro for logging information messages.
 */
#define LOGI(msg) LOG(lkCommon::Utils::Logger::LogLevel::INFO, msg)

/**
 * Macro for logging Warning messages.
 */
#define LOGW(msg) LOG(lkCommon::Utils::Logger::LogLevel::WARNING, msg)

/**
 * Macro for logging Error messages.
 */
#define LOGE(msg) LOG(lkCommon::Utils::Logger::LogLevel::ERROR, msg)
