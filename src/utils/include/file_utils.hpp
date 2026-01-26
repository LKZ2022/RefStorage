//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#pragma once

#include "common/common_types.hpp"
#include <filesystem>
#include <vector>
#include <set>

#include "LogMacros.hpp"

namespace RefStorage::Utils {

    class FileUtils {

    public:

        //用于比对文件夹是否相同：
        //文件信息
        struct FileInfo {

            std::string            relative_path;
            uintmax_t              file_size;
            std::filesystem::perms permissions;
            std::string            hash;

            bool operator<(const FileInfo& other) const {
                return relative_path < other.relative_path;
            }

        };

        //目录信息
        struct DirectoryInfo {

            std::string            relative_path;
            bool                   is_empty;
            std::filesystem::perms permissions;

            bool operator<(const DirectoryInfo& other) const {
                return relative_path < other.relative_path;
            }

        };


        //类方法：
        //读取文件
        static std::vector<char> read_file(const std::filesystem::path& filepath);

        // 创建目录（包括父目录）
        static bool create_directory(const std::filesystem::path& dirpath);
        // 向文件写入数据
        static bool write_file(const std::filesystem::path& filepath, const void* data, size_t length);

        // 追加数据到文件
        static bool append_file(const std::filesystem::path& filepath, const void* data, size_t length);

        // 删除文件
        static bool delete_file(const std::filesystem::path& filepath);

        // 获取文件的MIME类型
        static std::string get_mime_type(const std::filesystem::path& filepath);

        // 生成唯一的临时文件名
        static std::string generate_temp_filename(const std::string& prefix = "tmp_");

        // 比较两个文件/文件夹是否相同
        static bool compare_files(const std::filesystem::path& dir1, const std::filesystem::path& dir2);

    private:

        //辅助函数：
        // 收集文件夹中的所有文件信息
        static void collectFiles(const std::filesystem::path& base_dir,
                                          std::set<FileInfo>& file_infos,
                                     std::set<DirectoryInfo>& dir_infos);

        //权限转字符串
        static std::string permissionsToString(std::filesystem::perms p);

        // 比较文件信息
        static bool compareFileInfos(const FileInfo& info1, const FileInfo& info2);

        // 比较目录信息
        static bool compareDirInfos(const DirectoryInfo& info1, const DirectoryInfo& info2);

    };
}
