//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.


// include/log/Logger.hpp




#pragma once

#include "LogLevel.hpp"
#include "LogSink.hpp"
#include <string>
#include <memory>
#include <vector>
#include <mutex>

namespace Log
{

    // ǰ������
    struct Config;

    // ��־��������
    class Logger
    {
    public:
        // ��ȡ����
        static Logger& Instance();

        // ���ÿ������ƶ�
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        Logger(Logger&&) = delete;
        Logger& operator=(Logger&&) = delete;

        // �����ӿ�
        void Initialize(Level level = Level::LVL_INFO);
        void SetLevel(Level level);
        Level GetLevel() const;

        // ��־��¼��ͬ����
        void Log(Level level,
            const char* file,
            int line,
            const char* function,
            const std::string& message);

        // �������Ŀ��?
        void AddSink(std::unique_ptr<Sink> sink);

        // ��ݷ��������ӿ���̨���
        void AddConsoleSink(Level min_level = Level::LVL_INFO,
            bool use_colors = true,
            const std::string& pattern = "");

        // ��ݷ����������ļ����
        void AddFileSink(const std::string& filename,
            Level min_level = Level::LVL_INFO,
            const std::string& pattern = "");

        // ���ýӿ�
        void Initialize(const Config& config);
        void InitializeFromFile(const std::string& config_path);
        void InitializeFromEnv();

        // ˢ���������Ŀ��?
        void Flush();

        // �ݹ������Է���
        void RecursiveTest(int depth = 3);

    private:
        Logger();
        ~Logger();

        // ��������
        bool ShouldLog(Level level) const;

        // д������sink
        void WriteToSinks(const Message& msg);

        // �������ô���Sink
        std::unique_ptr<Sink> CreateSink(const struct SinkConfig& config);

    private:
        mutable std::recursive_mutex mutex_;
        std::vector<std::unique_ptr<Sink>> sinks_;
        Level current_level_{ Level::LVL_INFO };
        bool initialized_{ false };
    };

} // namespace Log
