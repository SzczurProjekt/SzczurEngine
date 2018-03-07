#pragma once

#if defined(NDEBUG)

#define INIT_LOGGER()
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_INFO_IF(...)
#define LOG_WARN_IF(...)
#define LOG_ERROR_IF(...)
#define LOG_INFO_IF_CX(...)
#define LOG_WARN_IF_CX(...)
#define LOG_ERROR_IF_CX(...)
#define ASSERT(...)
#define IF_EDITOR if constexpr(false)

#else

#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string_view>
#include <type_traits>

#include "Szczur/CompilerPortability.hpp"

namespace rat
{

#define ASSERT(message, expr) assert(!message && (expr))

class DebugLogger
{
public:

    template <typename... Ts>
    void log(const char* file, int line, Ts&&... args)
    {
        _formatTime("%H:%M:%S");

        std::string_view view = file;
        view = view.substr(view.find_last_of(DIRECTORY_SEPARATOR_CHAR) + 1);

        _logFile.open(_logFilePath, std::ios::app);
        _logFile << '[' << _buffer << ']' << ' ' << '[' << view << ':' << line << ']' << ' '; (_logFile << ... << std::forward<Ts>(args)); _logFile << '\n' << std::flush;
        _logFile.close();

        std::cerr << '[' << _buffer << ']' << ' ' << '[' << view << ':' << line << ']' << ' '; (std::cerr << ... << std::forward<Ts>(args)); std::cerr << '\n' << std::flush;
    }

private:

    void _formatTime(const char* format)
    {
        std::time_t tm = std::time(nullptr);

        std::strftime(_buffer, sizeof(_buffer), format, std::localtime(&tm));
    }

    char _buffer[64];
    std::string _logFilePath = "Logs/" + std::to_string(std::time(nullptr)) + ".log";
    std::ofstream _logFile;

};

inline DebugLogger* logger = nullptr;

}

#define INIT_LOGGER() rat::DebugLogger ratDebugLogger; rat::logger = &ratDebugLogger
#define LOG_INFO(...) { rat::logger->log(__FILE__, __LINE__, "[INFO] ", __VA_ARGS__); }
#define LOG_WARN(...) { rat::logger->log(__FILE__, __LINE__, "[WARN] ", __VA_ARGS__); }
#define LOG_ERROR(...) { rat::logger->log(__FILE__, __LINE__, "[ERROR] ", __VA_ARGS__); }
#define LOG_INFO_IF(condition, ...) { if(condition) LOG_INFO(__VA_ARGS__) }
#define LOG_WARN_IF(condition, ...) { if(condition) LOG_WARN(__VA_ARGS__) }
#define LOG_ERROR_IF(condition, ...) { if(condition) LOG_ERROR(__VA_ARGS__) }
#define LOG_INFO_IF_CX(condition, ...) { if constexpr(condition) LOG_INFO(__VA_ARGS__) }
#define LOG_WARN_IF_CX(condition, ...) { if constexpr(condition) LOG_WARN(__VA_ARGS__) }
#define LOG_ERROR_IF_CX(condition, ...) { if constexpr(condition) LOG_ERROR(__VA_ARGS__) }
#define IF_EDITOR if constexpr(true)

#endif