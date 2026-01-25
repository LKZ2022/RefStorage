//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#include "../include/file_utils.hpp"
#include <fstream>
#include "Log.hpp"

namespace RefStorage::Utils {

    //判断文件是否存在
    bool FileUtils::file_exists(const std::filesystem::path& filepath) {
        return std::filesystem::exists(filepath);
    }

    //获取文件大小
    FileSize FileUtils::get_file_size(const std::filesystem::path& filepath) {
        return std::filesystem::file_size(filepath);
    }

    //读取文件
    std::vector<char> FileUtils::read_file(const std::filesystem::path& filepath) {
        if (!file_exists(filepath)) {
            LOG_ERROR_FMT("文件不存在：{1}",  filepath.string());
            return {};
        }

        std::ifstream ifs(filepath, std::ios::binary | std::ios::ate);

        if (!ifs.is_open()) {
            LOG_ERROR_FMT("文件无法打开：{1}",  filepath.string());
            return {};
        }

        std::streamsize size = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (!ifs.read(buffer.data(), size)) {
            LOG_ERROR_FMT("文件读取失败：{1}",  filepath.string());
            return {};
        }

        return buffer;
    }

    // 从路径中提取目录
    std::string FileUtils::get_directory(const std::filesystem::path& filepath) {
        std::filesystem::path path(filepath);
        return path.parent_path().string();
    }

    //向文件写入数据
    bool FileUtils::write_file(const std::filesystem::path& filepath, const void* data, size_t size) {
        std::string dir = get_directory(filepath);



    }

}
