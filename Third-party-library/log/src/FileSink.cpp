//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

//	src/log/FileSink.cpp


#include "FileSink.hpp"
#include <iomanip>
#include <iostream>


namespace Log
{

    FileSink::FileSink(const std::string& filename, Level min_level, const std::string& pattern)
        : filename_(filename), min_level_(min_level)
    {

        if (pattern.empty())
        {
            formatter_ = std::make_unique<Formatter>();
        }
        else
        {
            formatter_ = std::make_unique<Formatter>(pattern);
        }

        EnsureFileOpen();
    }

    FileSink::~FileSink()
    {
        if (file_.is_open())
        {
            file_.close();
        }
    }

    void FileSink::Write(const Message& msg)
    {
        if (!ShouldLog(msg.level))
        {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);

        EnsureFileOpen();
        if (file_.is_open())
        {
            file_ << formatter_->Format(msg);
            //file_.flush();                                           //显示刷新，解决写入日志时的缓冲问题
        }
    }

    void FileSink::Flush()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (file_.is_open())
        {
            file_.flush();
        }
    }

    bool FileSink::ShouldLog(Level level) const
    {
        return level >= min_level_;
    }

    void FileSink::SetFormatter(std::unique_ptr<Formatter> formatter)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        formatter_ = std::move(formatter);
    }

    void FileSink::EnsureFileOpen()
    {
        if (!file_.is_open()) {
            // ����Ŀ¼����������ڣ�
            std::filesystem::path path(filename_);
            if (path.has_parent_path()) {
                std::filesystem::create_directories(path.parent_path());
            }

            file_.open(filename_, std::ios::app);
            if (!file_.is_open()) {
                std::cerr << "文件打开失败 " << filename_ << std::endl;
            }
        }
    }

} // namespace Log