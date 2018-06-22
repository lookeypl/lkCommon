#include "../Logger.hpp"

#include <fstream>
#include <iostream>


namespace lkCommon {
namespace Utils {
namespace Logger {

std::ofstream logFile;

void Log(LogLevel level, const std::stringstream& msg)
{
    const char* levelStr = nullptr;
    const char* colorStr = nullptr;

    if (!logFile.is_open())
    {
        logFile.open("log.txt");
    }

    switch (level)
    {
    case LogLevel::DEBUG:
        levelStr = "DEBUG";
        colorStr = "\033[36m"; // Cyan (Blue | Green)
        break;
    case LogLevel::INFO:
        levelStr = " INF ";
        colorStr = "\033[39m"; // Default
        break;
    case LogLevel::WARNING:
        levelStr = " WRN ";
        colorStr = "\033[33m"; // Yellow (Red | Green)
        break;
    case LogLevel::ERR:
        levelStr = "ERROR";
        colorStr = "\033[91m"; // Light red
        break;
    }

    std::stringstream fullMsg;
    fullMsg << colorStr << "[" << levelStr << "] " << msg.str() << "\033[39m\n";
    std::cout << fullMsg.str();
    if (logFile.is_open())
        logFile << fullMsg.str();
}

} // namespace Logger
} // namespace Utils
} // namespace lkCommon
