/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Logger module implementation
 */

#include "lkCommon/lkCommon.hpp"

#include "lkCommon/Utils/Logger.hpp"
#include "lkCommon/Utils/StringConv.hpp"

#include <Windows.h>
#include <fstream>
#include <iostream>

#undef ERROR


namespace {

std::ofstream gLogFile;
std::string gPathRoot;

} // namespace

namespace lkCommon {
namespace Utils {
namespace Logger {

void Log(LogLevel level, const char* file, uint32_t line, const std::string& msg)
{
    const char* levelStr = nullptr;

    static HANDLE console = 0;
    if (!console)
        console = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO conInfo;
    GetConsoleScreenBufferInfo(console, &conInfo);

    switch (level)
    {
    case LogLevel::DEBUG:
        levelStr = "[DEBUG] ";
        SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN);
        break;
    case LogLevel::INFO:
        levelStr = "[ INF ] ";
        break;
    case LogLevel::WARNING:
        levelStr = "[ WRN ] ";
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case LogLevel::ERR:
        levelStr = "[ERROR] ";
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    case LogLevel::MEMORY:
        levelStr = "[ MEM ] ";
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);
        break;
    case LogLevel::CALL:
        levelStr = "[ CALL ] ";
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
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
    fullMsg << levelStr
            << path << " @ " << line << ": "
            << msg << '\n';
    std::cout << fullMsg.str();
    if (gLogFile.is_open())
        gLogFile << fullMsg.str();

    SetConsoleTextAttribute(console, conInfo.wAttributes);

    std::wstring wideMsg;
    if (StringToWString(fullMsg.str(), wideMsg))
    {
        OutputDebugStringW(wideMsg.c_str());
    }
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
