//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#pragma once

#include "common/common_types.hpp"
#include <filesystem>
#include <vector>


namespace RefStorage::Utils {

    class FileUtils {

    public:
        //判断文件是否存在
        static bool file_exists(const std::filesystem::path& filepath);

        //获取文件大小
        static FileSize get_file_size(const std::filesystem::path& filepath);

        //读取文件
        static std::vector<char> read_file(const std::filesystem::path& filepath);

        // 从路径中提取目录
        static std::string get_directory(const std::filesystem::path& filepath);

        //向文件写入数据
        static bool write_file(const std::filesystem::path& filepath, const void* data, size_t size);

    };

}
