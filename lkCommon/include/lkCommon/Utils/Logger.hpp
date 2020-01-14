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
 * \li ERR - indicates an error message, colored in bright red. Name is
 * deliberately shorter to prevent collision with Windows API ERROR define.
 * \li WARNING - indicates a warning message, colored in bright yellow.
 * \li INFO - indicates a regular message, displayed without color enhancements.
 * \li DEBUG - indicates a debug message, colored in light blue. Debug logs are
 * disabled when building in Release mode (without _DEBUG define)
 * \li MEMORY - indicates a memory-related message (ex. marking allocation of
 * resources). This is useful when user wants to keep track of all resources
 * which are allocated by the program. MEMORY logs are off by default - to
 * enable them, define LKCOMMON_LOG_MEMORY preprocessor macro.
 * \li CALL - indicates a function call message (ex. logging input parameters).
 * CALL logs are off by default - to enable them, define LKCOMMON_LOG_CALL
 * preprocessor macro.
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
 *   * Evolve Logger backends to be more sophisticated (ex. to allow input)
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
    ERR = 0,
    WARNING,
    INFO,
    DEBUG,
    MEMORY,
    CALL,
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
void Log(LogLevel level, const char* file, uint32_t line, const std::string& msg);

/**
 * Sets project's root path to strip from log files.
 *
 * If below function is not called, all logs will print full path to project's root.
 *
 * @warning This function was designed to be called only once at application startup
 *          - it is NOT thread-safe.
 *
 * @p[in] path Project's root path to look for and strip.
 */
void SetRootPathToStrip(const std::string& path);

/**
 * Opens log file to additionally leave log informations there.
 *
 * @p[in] path Path to where a log file should be located
 *
 * @note If there was a log file previously opened, it will be closed before
 *       opening.
 *
 * @warning If file already exists, its contents are overwritten.
 */
void OpenLogFile(const std::string& path);

} // namespace Logger
} // namespace Utils
} // namespace lkCommon


#define LOG(level, msg) do { \
    std::stringstream ss; \
    ss << msg; \
    lkCommon::Utils::Logger::Log(level, __FILE__, __LINE__, ss.str()); \
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
 *
 * @todo Figure out another way to enable Memory logs (log levels?)
 */
#ifdef LKCOMMON_LOG_MEMORY
#define LOGM(msg) LOG(lkCommon::Utils::Logger::LogLevel::MEMORY, msg)
#else
#define LOGM(msg) do { } while(0)
#endif

/**
 * @def LOGC
 *
 * Macro used for logging function calls. Available only when
 * LKCOMMON_LOG_CALL macro is defined at build time.
 *
 * @todo Figure out another way to enable Call logs (log levels?)
 */
#ifdef LKCOMMON_LOG_CALL
#define LOGC(msg) LOG(lkCommon::Utils::Logger::LogLevel::CALL, msg)
#else
#define LOGC(msg) do { } while(0)
#endif

/**
 * @def LOGI
 *
 * Macro for logging information messages.
 */
#define LOGI(msg) LOG(lkCommon::Utils::Logger::LogLevel::INFO, msg)

/**
 * @def LOGW
 *
 * Macro for logging Warning messages.
 */
#define LOGW(msg) LOG(lkCommon::Utils::Logger::LogLevel::WARNING, msg)

/**
 * @def LOGE
 *
 * Macro for logging Error messages.
 */
#define LOGE(msg) LOG(lkCommon::Utils::Logger::LogLevel::ERR, msg)
