//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

// include/log/ConsoleSink.hpp



#pragma once

#include "LogSink.hpp"
#include "ColoredFormatter.hpp"
#include <mutex>
#include <iostream>

namespace Log
{

    class ConsoleSink : public Sink
    {
    public:
        ConsoleSink(Level min_level = Level::LVL_INFO,
            bool use_colors = true,
            const std::string& pattern = "");

        ~ConsoleSink() = default;

        void Write(const Message& msg) override;
        void Flush() override;
        bool ShouldLog(Level level) const override;
        void SetFormatter(std::unique_ptr<Formatter> formatter) override;

        // ������ɫģʽ
        void SetUseColors(bool use_colors);

    private:
#ifdef _WIN32
        void EnableANSIColorSupport() const;
#endif

    private:
        mutable std::mutex mutex_;
        Level min_level_;
        bool use_colors_;
        std::unique_ptr<ColoredFormatter> colored_formatter_;
        std::unique_ptr<Formatter> formatter_;
    };

} // namespace Log
