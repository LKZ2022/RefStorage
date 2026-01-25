//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#include "Log.hpp"
#include "hash_utils.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::cout << "=== 测试基础日志功能 ===\n\n";

    auto& logger = Log::Logger::Instance();
    logger.Initialize(Log::Level::LVL_DEBUG);

    // 添加输出
    logger.AddConsoleSink(Log::Level::LVL_DEBUG, true);
    logger.AddFileSink("app.log", Log::Level::LVL_DEBUG);

    // 测试各级别日志
    LOG_TRACE("TRACE消息 - 灰色");
    LOG_DEBUG("DEBUG消息 - 青色");
    LOG_INFO("INFO消息 - 绿色");
    LOG_WARN("WARN消息 - 黄色");
    LOG_ERROR("ERROR消息 - 红色");
    LOG_FATAL("FATAL消息 - 紫色");

    // 测试格式化消息
    LOG_INFO_FMT("格式化消息: {0}, {1}, {2}, {3}", 42, "hello", 3.14159, "world");


    std::cout << "\n\n";
    std::cout << "===测试哈希值计算功能===\n\n";

    std::string path1;
    RefStorage::Utils::HashUtils hashUtils;
    std::cout << "请输入文件路径：";

    std::getline(std::cin, path1);
    std::wstring widePath = std::wstring(path1.begin(), path1.end());
    std::filesystem::path file_path = path1;
    std::cout << "\n\n文件哈希为：";
    std::cout << RefStorage::Utils::HashUtils::calculateHash(file_path);



    std::string path2;
    std::cout << "\n\n请输入文件夹路径：";
    std::getline(std::cin, path2);
    file_path = path2;
    std::cout << RefStorage::Utils::HashUtils::calculateHash(file_path);

    system("pause");
}