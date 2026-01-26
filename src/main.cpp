//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#include "Log.hpp"
#include "hash_utils.hpp"
#include <iostream>

#include "file_utils.hpp"

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
        std::cout << "===测试文件比较功能===\n\n";
        std::filesystem::path path1;
        std::filesystem::path path2;
        std::cout << "请输入路径1：";
        std::cin >> path1;
        std::cout << "请输入路径2：";
        std::cin >> path2;
        if (RefStorage::Utils::FileUtils::compare_files(path1, path2)) {
            std::cout << "相同" << std::endl;
        }
        else {
            std::cout << "不同" << std::endl;
        }


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