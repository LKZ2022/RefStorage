//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#pragma once

#include <cstdint>
#include <chrono>
#include <string>
#include <vector>
#include <sqlite3.h>




namespace RefStorage {

    using FileSize  = std::uint64_t;                                           //文件大小
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;      //时间戳
    using HashValue = std::string;                                             //哈希值
    using NodeID    = std::uint32_t;                                           //节点ID
    using ChunkID   = std::uint64_t;                                           //分片ID
    using FileID    = std::uint64_t;                                           //文件ID

    namespace Common {

        //分片文件信息
        struct ChunkInfo {
            ChunkID             chunk_id_;
            HashValue           hash_value_;
            FileSize            file_size_;
            uint32_t            replica_count_;                                 //文件备份数量
            std::vector<NodeID> storage_node_;                                  //储存节点ID
            TimePoint           creat_time_;
        };

        //文件元数据
        struct FileMataData {
            FileID                  file_id_;
            std::string             file_name_;
            std::string             path;                                        //文件路径
            FileSize                file_size_;
            HashValue               hash_value_;
            std::string             mime_type_;                                  //文件MIME类型
            std::vector<ChunkInfo>  chunks_;                                     //分片信息
            uint32_t                reference_count_;                            //引用数（用户上传了多少哈希值相同的文件）
            TimePoint               creat_time_;
            TimePoint               last_access_time_;                           //最后上传时间

            //判断是已去重文件
            [[nodiscard]] bool isDeduplicated() const { return reference_count_ > 1; }
        };

        //储存节点（存储服务器）信息
        struct StorageNode {
            NodeID      node_id_;
            std::string node_address_;
            uint16_t    node_port_;                                              //储存节点端口
            FileSize    total_capacity_;                                         //储存节点总容量
            FileSize    used_capacity_;                                          //已使用存储
            bool        is_active;                                               //储存节点是否活动
            TimePoint   last_heartbeat;                                          //上次节点检测时间

            //计算节点储存利用率
            [[nodiscard]] double usage_ratio() const {
                return total_capacity_ > 0 ? static_cast<double>(used_capacity_) / total_capacity_ : 0.0;
            }
        };

        // 操作结果
        enum class StatusCode {
            OK                = 0,
            ERROR             = 1,
            FILE_NOT_FOUND    = 2,
            DUPLICATE_FILE    = 3,
            STORAGE_FULL      = 4,
            NETWORK_ERROR     = 5,
            PERMISSION_DENIED = 6,
            INVALID_ARGUMENT  = 7,
            DATABASE_ERROR    = 8
        };


        template <typename T>
        struct Result {
            StatusCode status_code_;
            std::string message_;
            T value_;

            // 构造函数
            Result() : status_code_(StatusCode::ERROR), message_("Uninitialized"), value_() {}
            Result(StatusCode code, const std::string& msg, const T& value) : status_code_(code), message_(msg), value_(value) {}       // 可拷贝类型
            Result(StatusCode code, const std::string& msg, T&& value) : status_code_(code), message_(msg), value_(std::move(value)) {} // 移动

            // 允许移动
            Result(Result&& other) noexcept : status_code_(other.status_code_), message_(std::move(other.message_)), value_(std::move(other.value_)) {}
            Result& operator=(Result&& other) noexcept {
                if (this != &other) {
                    status_code_ = other.status_code_;
                    message_ = std::move(other.message_);
                    value_ = std::move(other.value_);
                }
                return *this;
            }

            // 删除拷贝构造函数和拷贝赋值运算符
            Result(const Result&) = delete;
            Result& operator=(const Result&) = delete;

            //判断操作结果是否成功
            [[nodiscard]] bool success() const { return status_code_ == StatusCode::OK; }
            [[nodiscard]] bool failed() const { return !success(); }
            //
            static Result<T> Success(T val) {
                return {StatusCode::OK, "Success", std::move(val)};
            }

            static Result<T> Error(StatusCode code, const std::string& msg) {
                // 对于 unique_ptr 特化处理
                if constexpr (std::is_same_v<T, std::unique_ptr<sqlite3_stmt, void(*)(sqlite3_stmt*)>>) {

                    auto deleter = [](sqlite3_stmt* stmt) {
                        if (stmt) sqlite3_finalize(stmt);
                    };

                    T empty_ptr(nullptr, deleter);

                    return {code, msg, std::move(empty_ptr)};
                } else {
                    // 其他类型使用原来的方式
                    return {code, msg, T{}};
                }
            }
        };


    }

}