//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#include "Log.hpp"
#include "hash_utils.hpp"
#include <fstream>
#include <openssl/evp.h>
#include <algorithm>
#include <functional>
#include <sstream>
#include <vector>
#include <windows.h>

namespace RefStorage::Utils {

    //字节数组转换为十六进制字符串（表示哈希值）
    std::string HashUtils::bytesToHex(const unsigned char* bytes, size_t length) {
        std::stringstream ss;

        ss << std::hex << std::setfill('0');
        for (size_t i = 0; i < length; i++) {
            ss << std::setw(2) << static_cast<int>(bytes[i]);
        }

        return ss.str();
    }


    //计算文件哈希值
    std::string HashUtils::calculateFileHash(const std::filesystem::path &filePath) {
        std::ifstream file(filePath, std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("无法打开文件: " + filePath.string());
        }
        //创建哈希上下文
        EVP_MD_CTX* loc_ctx = EVP_MD_CTX_new();
        //局部缓冲区
        char loc_buffer[4096] = {0};

        EVP_DigestInit_ex(loc_ctx, EVP_sha256(), nullptr);

        while (file.read(loc_buffer, sizeof(loc_buffer))) {
            EVP_DigestUpdate(loc_ctx, loc_buffer, file.gcount());
        }
        EVP_DigestUpdate(loc_ctx, loc_buffer, file.gcount());

        unsigned char loc_hash[EVP_MAX_MD_SIZE];
        EVP_DigestFinal_ex(loc_ctx, loc_hash, nullptr);

        EVP_MD_CTX_free(loc_ctx);

        return bytesToHex(loc_hash, EVP_MAX_MD_SIZE);
    }


    //计算文件夹哈希值
    std::string HashUtils::calculateFolderHash(const std::filesystem::path &folderPath) {
        std::vector<std::filesystem::path> loc_entries;
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            loc_entries.push_back(entry.path());
        }

        std::sort(loc_entries.begin(), loc_entries.end());

        EVP_MD_CTX* loc_ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(loc_ctx, EVP_sha256(), nullptr);

        for (const auto& loc_folder : loc_entries) {
            std::string loc_relativePath = loc_folder.filename().string();
            //计算相对路径哈希
            EVP_DigestUpdate(loc_ctx, loc_relativePath.c_str(), loc_folder.string().size());

            if (std::filesystem::is_directory(loc_folder)) {
                //对于子文件夹，递归计算哈希值
                std::string subHash = calculateFolderHash(loc_folder);
                EVP_DigestUpdate(loc_ctx, subHash.c_str(), subHash.size());
                //添加标记，区分文件/文件夹
                EVP_DigestUpdate(loc_ctx, "DIR", 3);
            }
            else if (!std::filesystem::is_directory(loc_folder)) {
                // 对于文件，计算文件内容的哈希值
                std::string fileHash = calculateFileHash(loc_folder);
                EVP_DigestUpdate(loc_ctx, fileHash.c_str(), fileHash.size());
                //添加标记，区分文件/文件夹
                EVP_DigestUpdate(loc_ctx, "FILE", 4);
            }

        }

        unsigned char loc_hash[EVP_MAX_MD_SIZE];
        EVP_DigestFinal_ex(loc_ctx, loc_hash, nullptr);

        EVP_MD_CTX_free(loc_ctx);

        return bytesToHex(loc_hash, EVP_MAX_MD_SIZE);
    }


    //计算文件或文件夹的哈希值
    std::string HashUtils::calculateHash(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) {
            LOG_ERROR_FMT("路径不存在: {0}", path.string());
            throw std::runtime_error("路径不存在: " + path.string());
        }

        HashUtils hashUtils;

        if (std::filesystem::is_directory(path)) {
            LOG_DEBUG("执行了计算文件夹哈希函数");
            return hashUtils.calculateFolderHash(path);
        }
        else if (std::filesystem::is_regular_file(path)) {
            LOG_DEBUG("执行了计算文件哈希值函数");
            return hashUtils.calculateFileHash(path);
        }
        else {
            LOG_ERROR_FMT("不支持的文件类型: {0}", path.string());
            throw std::runtime_error("不支持的文件类型: " + path.string());
        }
    }

    //判断两个哈希值是否相等
    bool HashUtils::isEqualHash(const std::string& hash1, const std::string& hash2) {
        return hash1 == hash2;
    }


}