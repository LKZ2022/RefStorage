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
    auto& logger = Log::Logger::Instance();
    logger.Initialize(Log::Level::LVL_DEBUG);

    // 添加输出
    logger.AddConsoleSink(Log::Level::LVL_DEBUG, true);
    logger.AddFileSink("app.log", Log::Level::LVL_DEBUG);

    int exitCode = 0;

    try {
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
    }
    catch (const std::runtime_error& e) {
        std::cerr << "运行时错误: " << e.what() << '\n';
        exitCode = 101;
    }
    catch (const std::logic_error& e) {
        std::cerr << "逻辑错误: " << e.what() << '\n';
        exitCode = 102;
    }
    catch (const std::exception& e) {
        std::cerr << "标准异常: " << e.what() << '\n';
        exitCode = 100;
    }
    catch (...) {
        std::cerr << "未知异常!\n";
        exitCode = -1;
    }

    return exitCode;
}