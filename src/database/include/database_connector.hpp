//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.
#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "common/common_types.hpp"


// SQLite3前向声明
struct sqlite3;
struct sqlite3_stmt;

namespace RefStorage::DataBase {

    class DatabaseException : public std::runtime_error {
    public:
        explicit DatabaseException(const std::string& message) : runtime_error(message) {}
    };

    class DatabaseConnector {
    public:
        explicit DatabaseConnector(const std::string& database_path = ":memory:");
        ~DatabaseConnector();

        //禁止复制
        DatabaseConnector(const DatabaseConnector&) = delete;
        DatabaseConnector& operator=(const DatabaseConnector&) = delete;
        //允许移动
        DatabaseConnector(DatabaseConnector&& other) noexcept;
        DatabaseConnector& operator=(DatabaseConnector&& other) noexcept;

        //连接管理
        bool connect(const std::string& database_path);
        void disconnect();
        bool is_connected() const;

        //执行SQL语句
        Common::Result<bool> execute(const std::string& sql);

        //执行查询，返回结果集
        template <typename T>
        Common::Result<std::vector<T>> query(const std::string& sql, std::function<T(sqlite3_stmt*)> row_mapper);

        // 开始事务
        Common::Result<bool> begin_transaction();

        // 提交事务
        Common::Result<bool> commit_transaction();

        // 回滚事务
        Common::Result<bool> rollback_transaction();

        //获取最后插入的行ID
        int64_t get_last_inserted_rowid() const;

        //获取受影响的行数
        int get_changes_count() const;

        //准备预处理数据
        Common::Result<std::unique_ptr<sqlite3_stmt, void(*)(sqlite3_stmt*)>> prepare_statement(const std::string& sql);

    private:
        void handle_sqlite_error(int error_code, const std::string& operation);

        sqlite3*    database_;
        std::string database_path_;
        bool        in_transaction_;

    };

}
