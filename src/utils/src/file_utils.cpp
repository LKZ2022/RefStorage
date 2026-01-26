//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#include "file_utils.hpp"
#include <fstream>
#include <random>
#include <unordered_map>
#include "hash_utils.hpp"
#include "Log.hpp"

namespace RefStorage::Utils {

    std::vector<char> FileUtils::read_file(const std::filesystem::path& filepath) {
        if (!std::filesystem::exists(filepath)) {
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

    bool FileUtils::create_directory(const std::filesystem::path& dirpath) {
        if (dirpath.empty()) {
            return true;
        }

        try {
            return std::filesystem::create_directory(dirpath);
        }catch(const std::filesystem::filesystem_error& e) {
            LOG_ERROR_FMT("创建目录失败：{0}，错误：{1}", dirpath.string(), e.what());
            return false;
        }catch (...) {
            LOG_ERROR_FMT("创建目录失败：{0}，未知错误", dirpath.string());
            return false;
        }
    }

    bool FileUtils::write_file(const std::filesystem::path& filepath, const void* data, size_t length) {
        std::string dir = filepath.parent_path().string();
        if (!dir.empty() && !create_directory(dir)) {
            LOG_ERROR_FMT("无法为文件创建目录：{0}", dir);
            return false;
        }

        std::ofstream ofs(filepath, std::ios::binary);
        if (!ofs.is_open()) {
            LOG_ERROR_FMT("无法打开文件进行写入：{0}", filepath.string());
            return false;
        }

        ofs.write(static_cast<const char*>(data), length);
        if (!ofs.good()) {
            LOG_ERROR_FMT("写入数据到文件失败：{0}", filepath.string());
            return false;
        }

        LOG_INFO_FMT("成功将数据写入文件：{0}", filepath.string());
        return true;
    }

    bool FileUtils::append_file(const std::filesystem::path& filepath, const void* data, size_t length) {

        std::ofstream file(filepath, std::ios::binary | std::ios::app);
        if (!file.is_open()) {
            LOG_ERROR("无法打开文件以追加内容：{0} " + filepath.string());
            return false;
        }

        file.write(static_cast<const char*>(data), length);
        if (!file.good()) {
            LOG_ERROR("无法向文件追加数据：{0} " + filepath.string());
            return false;
        }

        LOG_INFO_FMT("成功向文件将追加数据：{0}", filepath.string());
        return true;

    }

    bool FileUtils::delete_file(const std::filesystem::path& filepath) {
        try {
            return std::filesystem::remove(filepath);
        } catch (const std::filesystem::filesystem_error& e) {
            LOG_ERROR_FMT("删除文件失败：{0}，错误：{1}", filepath.string(), e.what());
            return false;
        }
    }

    std::string FileUtils::get_mime_type(const std::filesystem::path& filepath) {
        std::string extension = filepath.extension().string();

        //常见MIME类型映射
        static const std::unordered_map<std::string, std::string> mime_types = {
            {".txt" , "text/plain"},
            {".html", "text/html"},
            {".htm" , "text/html"},
            {".css" , "text/css"},
            {".js"  , "application/javascript"},
            {".json", "application/json"},
            {".xml" , "application/xml"},
            {".pdf" , "application/pdf"},
            {".zip" , "application/zip"},
            {".tar" , "application/x-tar"},
            {".gz"  , "application/gzip"},
            {".jpg" , "image/jpeg"},
            {".jpeg", "image/jpeg"},
            {".png" , "image/png"},
            {".gif" , "image/gif"},
            {".bmp" , "image/bmp"},
            {".svg" , "image/svg+xml"},
            {".ico" , "image/x-icon"},
            {".mp3" , "audio/mpeg"},
            {".wav" , "audio/wav"},
            {".mp4" , "video/mp4"},
            {".avi" , "video/x-msvideo"},
            {".mov" , "video/quicktime"},
            {".webm", "video/webm"},
            {".doc" , "application/msword"},
            {".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
            {".xls" , "application/vnd.ms-excel"},
            {".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
            {".ppt" , "application/vnd.ms-powerpoint"},
            {".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
        };

        auto it = mime_types.find(extension);

        if (it != mime_types.end()) {
            return it->second;
        }

        return "application/octet-stream";

    }

    std::string FileUtils::generate_temp_filename(const std::string& prefix) {
        //获取当前高精度时间
        auto now = std::chrono::system_clock::now();
        //获取时间戳
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

        //当前时间+随机生成文件名
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1000, 9999);
        int random = dis(gen);

        return prefix + std::to_string(timestamp) + "_" + std::to_string(random) + ".tmp";
    }

    void FileUtils::collectFiles(const std::filesystem::path& base_dir,
                                          std::set<FileInfo>& file_infos,
                                     std::set<DirectoryInfo>& dir_infos) {
        if (!std::filesystem::exists(base_dir) || !std::filesystem::is_directory(base_dir)) {
            LOG_ERROR_FMT("目录不存在或不是文件夹: {0}", base_dir);
            throw std::runtime_error("目录不存在或不是文件夹: " + base_dir.string());
        }

        for (const auto &entry: std::filesystem::recursive_directory_iterator(
                 base_dir, std::filesystem::directory_options::skip_permission_denied)) {

            try {

                std::filesystem::path full_path = entry.path();
                std::filesystem::path relative_path = std::filesystem::relative(full_path, base_dir);

                //获取文件信息
                if (std::filesystem::is_regular_file(entry.status())) {

                    FileInfo info;

                    info.relative_path = relative_path.string();
                    info.file_size = std::filesystem::file_size(full_path);
                    info.permissions = entry.status().permissions();
                    info.hash = HashUtils::calculateHash(full_path);

                    file_infos.insert(info);

                }
                //获取文件夹信息
                else if (std::filesystem::is_directory(entry.status())) {
                    DirectoryInfo dir_info;
                    dir_info.relative_path = relative_path.string();
                    dir_info.is_empty = std::filesystem::is_empty(full_path);
                    dir_info.permissions = entry.status().permissions();

                    dir_infos.insert(dir_info);
                }
                //暂时只考虑文件和文件夹

            }catch (std::exception& e) {
                LOG_ERROR_FMT("警告: 跳过无法访问的条目: {0} ({1})", entry.path(), e.what());
                continue;
            }

        }


    }

    bool FileUtils::compareFileInfos(const FileInfo& info1, const FileInfo& info2) {
        // 检查路径
        if (info1.relative_path != info2.relative_path) {
            return false;
        }

        // 检查文件大小
        if (info1.file_size != info2.file_size) {
            return false;
        }

        // 检查元数据
        if (info1.permissions != info2.permissions) {
            return false;
        }

        // 检查哈希值
        if (!info1.hash.empty() && !info2.hash.empty()) {
            return info1.hash == info2.hash;
        }

        return true;
    }

    bool FileUtils::compareDirInfos(const DirectoryInfo& info1, const DirectoryInfo& info2) {
        if (info1.relative_path != info2.relative_path) {
            return false;
        }

        // 检查是否都为空或都不为空
        if (info1.is_empty != info2.is_empty) {
            return false;
        }

        // 检查元数据（如果启用）
        if (info1.permissions != info2.permissions) {
            return false;
        }

        return true;
    }

    bool FileUtils::compare_files(const std::filesystem::path &dir1, const std::filesystem::path &dir2) {

        try {
            // 检查文件夹是否存在
            if (!std::filesystem::exists(dir1) || !std::filesystem::exists(dir2)) {
                LOG_ERROR("错误: 一个或两个文件夹不存在");
                return false;
            }

            // 收集两个文件夹的信息
            std::set<FileInfo> files1, files2;
            std::set<DirectoryInfo> dirs1, dirs2;

            LOG_INFO("正在扫描文件夹1:");
            collectFiles(dir1, files1, dirs1);
            LOG_INFO_FMT("找到{0} 个文件, {1} 个文件夹", files1.size(), dirs1.size());

            LOG_INFO("正在扫描文件夹2:");
            collectFiles(dir2, files2, dirs2);
            LOG_INFO_FMT("找到{0} 个文件, {1} 个文件夹", files2.size(), dirs2.size());

            // 比较文件内容
            auto it1 = files1.begin();
            auto it2 = files2.begin();

            while (it1 != files1.end() && it2 != files2.end()) {
                if (!compareFileInfos(*it1, *it2)) {
                    LOG_INFO_FMT("文件不同:{0} ",it1->relative_path);
                    LOG_INFO_FMT("文件1信息：\n相对路径：{0}文件大小：{1}权限：{2}哈希值：{3}", it1->relative_path, it1->file_size, permissionsToString(it1->permissions), it1->hash);
                    LOG_INFO_FMT("文件1信息：\n相对路径：{0}文件大小：{1}权限：{2}哈希值：{3}", it2->relative_path, it2->file_size, permissionsToString(it2->permissions), it2->hash);
                    return false;
                }
                ++it1;
                ++it2;
            }

            // 比较文件夹
            auto dir_it1 = dirs1.begin();
            auto dir_it2 = dirs2.begin();

            while (dir_it1 != dirs1.end() && dir_it2 != dirs2.end()) {
                if (!compareDirInfos(*dir_it1, *dir_it2)) {
                    LOG_INFO_FMT("文件夹不同:{0} ", dir_it1->relative_path);
                    return false;
                }
                ++dir_it1;
                ++dir_it2;
            }

            LOG_INFO("文件夹完全相同");
            return true;

        }catch (std::exception& e) {
            LOG_INFO_FMT("比较过程中出错: {0}", e.what());
            return false;
        }

    }

    std::string FileUtils::permissionsToString(std::filesystem::perms p) {
        std::string result;

        result.push_back((p & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? 'r' : '-');
        result.push_back((p & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? 'w' : '-');
        result.push_back((p & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? 'x' : '-');
        result.push_back((p & std::filesystem::perms::group_read) != std::filesystem::perms::none ? 'r' : '-');
        result.push_back((p & std::filesystem::perms::group_write) != std::filesystem::perms::none ? 'w' : '-');
        result.push_back((p & std::filesystem::perms::group_exec) != std::filesystem::perms::none ? 'x' : '-');
        result.push_back((p & std::filesystem::perms::others_read) != std::filesystem::perms::none ? 'r' : '-');
        result.push_back((p & std::filesystem::perms::others_write) != std::filesystem::perms::none ? 'w' : '-');
        result.push_back((p & std::filesystem::perms::others_exec) != std::filesystem::perms::none ? 'x' : '-');

        return result;
    }
}
