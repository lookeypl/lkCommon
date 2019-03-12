/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Logger module implementation
 */

#include "lkCommon/Utils/Logger.hpp"

#include <fstream>
#include <iostream>


namespace lkCommon {
namespace Utils {
namespace Logger {

// TODO move to class
std::ofstream gLogFile;
std::string gPathRoot;

void Log(LogLevel level, const char* file, uint32_t line, const std::stringstream& msg)
{
    const char* levelStr = nullptr;
    const char* colorStr = nullptr;

    switch (level)
    {
    case LogLevel::DEBUG:
        levelStr = " DBG ";
        colorStr = "\033[36m"; // Cyan (Blue | Green)
        break;
    case LogLevel::INFO:
        levelStr = " INF ";
        colorStr = "\033[39m"; // Default
        break;
    case LogLevel::WARNING:
        levelStr = " WRN ";
        colorStr = "\033[93m"; // Bright Yellow (Red | Green)
        break;
    case LogLevel::ERR:
        levelStr = "ERROR";
        colorStr = "\033[91m"; // Light red
        break;
    case LogLevel::MEMORY:
        levelStr = " MEM ";
        colorStr = "\033[33m"; // Yellow (Red | Green)
    }

    std::string path;
    if (!gPathRoot.empty())
    {
        // strip as much of the path as possible
        char* newFilePtr = const_cast<char*>(file);

        uint32_t i = 0;
        while (newFilePtr[i] && (newFilePtr[i] == gPathRoot[i]))
            ++i;

        path = &(newFilePtr[i]);
    }
    else
    {
        path = file;
    }

    std::stringstream fullMsg;
    fullMsg << colorStr << "[" << levelStr << "] "
            << path << " @ " << line << ": "
            << msg.str() << "\033[39m\n";
    std::cout << fullMsg.str();
    if (gLogFile.is_open())
        gLogFile << fullMsg.str();
}

void SetRootPathToStrip(const std::string& path)
{
    gPathRoot = path;
}

void OpenLogFile(const std::string& path)
{
    if (gLogFile.is_open())
        gLogFile.close();

    gLogFile.open(path);
}

} // namespace Logger
} // namespace Utils
} // namespace lkCommon
