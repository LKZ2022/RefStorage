//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

// include/log/LogMessage.hpp
#pragma once

#include "LogLevel.hpp"
#include <chrono>
#include <string>
#include <thread>

namespace Log
{

    // ��־��Ϣ�ṹ
    struct Message
    {
        std::chrono::system_clock::time_point timestamp;
        Level level;
        std::string file;
        int line;
        std::string function;
        std::thread::id thread_id;
        std::string content;

        // ���캯��
        Message(Level lvl, const char* f, int l,
            const char* func, std::string_view msg)
            : timestamp(std::chrono::system_clock::now())
            , level(lvl)
            , file(f)
            , line(l)
            , function(func ? func : "")
            , thread_id(std::this_thread::get_id())
            , content(msg) {
        }
    };

} // namespace Log
