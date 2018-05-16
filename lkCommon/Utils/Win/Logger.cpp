#include "PCH.hpp"
#include "lkCommon.hpp"

#include "Utils/Logger.hpp"
#include "Utils/UTF.hpp"

#include <fstream>

namespace lkCommon {
namespace Logger {

std::ofstream logFile;

void Log(LogLevel level, const std::stringstream& msg)
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
        levelStr = "DEBUG";
        SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN);
        break;
    case LogLevel::INFO:
        levelStr = " INF ";
        break;
    case LogLevel::WARNING:
        levelStr = " WRN ";
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case LogLevel::ERROR:
        levelStr = "ERROR";
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    case LogLevel::MEMORY:
        levelStr = " MEM ";
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);
    }

    std::stringstream fullMsg;
    fullMsg << "[" << levelStr << "] " << msg.str() << "\n";
    std::cout << fullMsg.str();
    if (logFile.is_open())
        logFile << fullMsg.str();

    SetConsoleTextAttribute(console, conInfo.wAttributes);

    std::wstring wideMsg;
    UTF8ToUTF16(fullMsg.str(), wideMsg);

    OutputDebugStringW(wideMsg.c_str());
}

} // namespace Logger
} // namespace lkCommon
