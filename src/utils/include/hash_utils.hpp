//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#pragma once

#include <string>
#include<filesystem>



namespace RefStorage::Utils {
    class HashUtils {
    private:

        //计算文件哈希值
        std::string calculateFileHash(const std::filesystem::path& filePath);

        //计算文件夹哈希值
        std::string calculateFolderHash(const std::filesystem::path& folderPath);

        //字节数组转换为十六进制字符串（表示哈希值）
        std::string bytesToHex(const unsigned char* bytes, size_t length);

    public:

        //计算文件或文件夹哈希值
        static std::string calculateHash(const std::filesystem::path& path);

        //判断两个哈希值是否相等
        static bool isEqualHash(const std::string& hash1, const std::string& hash2);

    };
}