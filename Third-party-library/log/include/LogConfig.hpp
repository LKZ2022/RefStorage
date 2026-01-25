//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

// include/log/LogConfig.hpp



#pragma once

#include "LogLevel.hpp"
#include <string>
#include <vector>
#include <map>

namespace Log
{

    // Sink����
    struct SinkConfig
    {
        std::string type;  // "console", "file"
        Level min_level{ Level::LVL_INFO };

        // ͨ������
        std::string pattern;  // ��ʽ��ģʽ

        // ����̨����
        bool use_colors{ true };

        // �ļ�����
        std::string filename;
        size_t max_size{ 100 * 1024 * 1024 };  // 100MB
        int max_files{ 10 };

        SinkConfig() = default;
        SinkConfig(const std::string& t) : type(t) {}
    };

    // ��־����
    struct Config
    {
        Level global_level{ Level::LVL_INFO };
        std::vector<SinkConfig> sinks;
        std::map<std::string, Level> module_levels;

        // ���ļ�����
        static Config LoadFromFile(const std::string& path);

        // �ӻ�����������
        static Config LoadFromEnv();

        // Ĭ������
        static Config DefaultConfig();
    };

} // namespace Log
