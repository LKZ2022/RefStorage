//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#include "ConsoleSink.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace Log
{

    ConsoleSink::ConsoleSink(Level min_level, bool use_colors, const std::string& pattern)
        : min_level_(min_level), use_colors_(use_colors)
    {

        if (pattern.empty())
        {
            colored_formatter_ = std::make_unique<ColoredFormatter>();
        }
        else
        {
            colored_formatter_ = std::make_unique<ColoredFormatter>(pattern);
        }

        colored_formatter_->SetUseColors(use_colors);

#ifdef _WIN32
        if (use_colors)
        {
            EnableANSIColorSupport();
        }
#endif
    }

    void ConsoleSink::Write(const Message& msg)
    {
        if (!ShouldLog(msg.level))
        {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);

        std::string formatted = colored_formatter_->Format(msg);

        // ѡ�������
        if (msg.level >= Level::LVL_ERROR)
        {
            std::cerr << formatted;
            std::cerr.flush();
        }
        else
        {
            std::cout << formatted;
            std::cout.flush();
        }
    }

    void ConsoleSink::SetFormatter(std::unique_ptr<Formatter> formatter)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        formatter_ = std::move(formatter);
    }

    void ConsoleSink::Flush()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout.flush();
        std::cerr.flush();
    }

    bool ConsoleSink::ShouldLog(Level level) const
    {
        return level >= min_level_;
    }

    void ConsoleSink::SetUseColors(bool use_colors)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        use_colors_ = use_colors;
        colored_formatter_->SetUseColors(use_colors);
    }

#ifdef _WIN32
    void ConsoleSink::EnableANSIColorSupport() const
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole != INVALID_HANDLE_VALUE)
        {
            DWORD mode;
            if (GetConsoleMode(hConsole, &mode))
            {
                mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hConsole, mode);
            }
        }

        // ͬʱҲΪ�����������ANSI��ɫ֧��
        hConsole = GetStdHandle(STD_ERROR_HANDLE);
        if (hConsole != INVALID_HANDLE_VALUE)
        {
            DWORD mode;
            if (GetConsoleMode(hConsole, &mode))
            {
                mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hConsole, mode);
            }
        }
    }
#endif

} // namespace Log