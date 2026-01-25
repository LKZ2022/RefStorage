//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

// include/log/FileSink.hpp



#pragma once

#include "LogSink.hpp"
#include "LogFormatter.hpp"
#include <fstream>
#include <mutex>
#include <filesystem>

namespace Log
{

    class FileSink : public Sink
    {
    public:
        explicit FileSink(const std::string& filename,
            Level min_level = Level::LVL_INFO,
            const std::string& pattern = "");
        ~FileSink();

        void Write(const Message& msg) override;
        void Flush() override;
        bool ShouldLog(Level level) const override;
        void SetFormatter(std::unique_ptr<Formatter> formatter) override;

    private:
        // ȷ���ļ���
        void EnsureFileOpen();

    private:
        mutable std::mutex mutex_;
        std::ofstream file_;
        std::string filename_;
        Level min_level_;
        std::unique_ptr<Formatter> formatter_;
    };

} // namespace Log
