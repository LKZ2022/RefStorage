//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

// src/log/LogLevel.cpp

#include "LogLevel.hpp"

namespace Log {

    const char* LevelToString(Level level)
    {
        switch (level)
        {
        case Level::LVL_TRACE: return "TRACE";
        case Level::LVL_DEBUG: return "DEBUG";
        case Level::LVL_INFO:  return "INFO ";
        case Level::LVL_WARN:  return "WARN ";
        case Level::LVL_ERROR: return "ERROR";
        case Level::LVL_FATAL: return "FATAL";
        default: return "UNKNOWN";
        }
    }


    const char* LevelToShortString(Level level)
    {
        switch (level)
        {
        case Level::LVL_TRACE: return "T";
        case Level::LVL_DEBUG: return "D";
        case Level::LVL_INFO:  return "I";
        case Level::LVL_WARN:  return "W";
        case Level::LVL_ERROR: return "E";
        case Level::LVL_FATAL: return "F";
        default: return "?";
        }
    }

} // namespace Log