//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#include "../include/database_connector.hpp"
#include <sqlite3.h>
#include "Log.hpp"


namespace RefStorage::DataBase {

    DatabaseConnector::DatabaseConnector(const std::string &database_path)
        : database_(nullptr)
        , database_path_(database_path)
        , in_transaction_(false) {
        if (!database_path.empty()) {
            connect(database_path_);
        }
    }

    DatabaseConnector::~DatabaseConnector() {
        disconnect();
    }

    DatabaseConnector::DatabaseConnector(DatabaseConnector &&other) noexcept
        : database_(other.database_)
        , database_path_(std::move(other.database_path_))
        , in_transaction_(other.in_transaction_) {
        other.database_       = nullptr;
        other.in_transaction_ = false;
    }

    DatabaseConnector& DatabaseConnector::operator=(DatabaseConnector &&other) noexcept {
        if (this != &other) {
            disconnect();
            database_             = other.database_;
            database_path_        = std::move(other.database_path_);
            in_transaction_       = other.in_transaction_;
            other.database_       = nullptr;
            other.in_transaction_ = false;
        }

        return *this;
    }

    bool DatabaseConnector::connect(const std::string& database_path) {
        if (!database_path.empty()) {
            database_path_ = database_path;
        }

        if (in_transaction_) {
            disconnect();
        }

        int rc = sqlite3_open_v2(
            database_path_.c_str(), &database_,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI,
            nullptr);

        if (rc != SQLITE_OK) {
            handle_sqlite_error(rc,"connect");
            return false;
        }

        sqlite3_busy_timeout(database_, 5000);

        LOG_INFO_FMT("已连接到数据库：{1}", database_path_);
        return true;
    }

    void DatabaseConnector::disconnect() {
        if (database_ != nullptr) {
            if (in_transaction_) {
                DatabaseConnector::rollback_transaction();
            }

            sqlite3_close(database_);
            database_ = nullptr;
            LOG_INFO("已断开连接数据库");
        }
    }

    bool DatabaseConnector::is_connected() const {
        return database_ != nullptr;
    }

    Common::Result<bool> DatabaseConnector::execute(const std::string &sql) {
        if (!is_connected()) {
            return Common::Result<bool>::Error(Common::StatusCode::DATABASE_ERROR,"数据库未连接");
        }

        char *error_msg = nullptr;
        int rc = sqlite3_exec(database_, sql.c_str(), nullptr, nullptr, &error_msg);

        if (rc != SQLITE_OK) {
            std::string error = error_msg ? error_msg : "未知错误";
            handle_sqlite_error(rc,"执行：" + sql);

            return Common::Result<bool>::Error(Common::StatusCode::DATABASE_ERROR, "SQL执行时错误：" + error);
        }

        return Common::Result<bool>::Success(true);
    }

    template<typename T>
    Common::Result<std::vector<T> > DatabaseConnector::query(const std::string& sql, std::function<T(sqlite3_stmt *)> row_mapper) {
        if (!is_connected()) {
            return Common::Result<std::vector<T>>::Error(Common::StatusCode::DATABASE_ERROR, "数据库未连接");
        }

        sqlite3_stmt *stmt = nullptr;
        int rc = sqlite3_prepare_v2(database_, sql.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK) {
            handle_sqlite_error(rc,"准备查询：" + sql);
            return Common::Result<std::vector<T>>::Error(Common::StatusCode::DATABASE_ERROR, "SQL语句准备失败");
        }

        std::vector<T> results;

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            try {
                results.push_back(row_mapper(stmt));
            }catch(const std::exception& e) {
                sqlite3_finalize(stmt);
                return Common::Result<std::vector<T>>::Error(Common::StatusCode::DATABASE_ERROR,
                                                             std::string(e.what()));
            }
        }

        if (rc != SQLITE_DONE) {
            handle_sqlite_error(rc, "查询停止");
            sqlite3_finalize(stmt);
            return Common::Result<std::vector<T>>::Error(Common::StatusCode::DATABASE_ERROR, "执行查询失败");
        }

        sqlite3_finalize(stmt);
        return Common::Result<std::vector<T>>::Success(results);
    }

    // 显式实例化常用类型
    template Common::Result<std::vector<int> > DatabaseConnector::query<int>(const std::string& sql, std::function<int(sqlite3_stmt*)> row_mapper);
    template Common::Result<std::vector<std::string> > DatabaseConnector::query<std::string>(const std::string& sql, std::function<std::string(sqlite3_stmt*)> row_mapper);

    Common::Result<bool> DatabaseConnector::begin_transaction() {
        if (in_transaction_) {
            return Common::Result<bool>::Error(Common::StatusCode::DATABASE_ERROR, "事务已开始");
        }

        auto result = execute("BEGIN TRANSACTION");
        if (result.success()) {
            in_transaction_ = true;
        }

        return result;
    }

    Common::Result<bool> DatabaseConnector::commit_transaction() {
        if (!in_transaction_) {
            return Common::Result<bool>::Error(Common::StatusCode::DATABASE_ERROR, "没有正在进行的事务");
        }

        auto result = execute("COMMIT TRANSACTION");

        if (result.success()) {
            in_transaction_ = false;
        }
        return result;
    }

    Common::Result<bool> DatabaseConnector::rollback_transaction() {
        if (!in_transaction_) {
            return Common::Result<bool>::Error(Common::StatusCode::DATABASE_ERROR, "没有事务正在进行");
        }

        auto result = execute("ROLLBACK TRANSACTION");

        if (result.success()) {
            in_transaction_ = false;
        }
        return result;

    }

    int64_t DatabaseConnector::get_last_inserted_rowid() const {
        if (!is_connected()) {
            return -1;
        }
        return sqlite3_last_insert_rowid(database_);
    }

    int DatabaseConnector::get_changes_count() const {
        if (!is_connected()) {
            return 0;
        }
        return sqlite3_changes(database_);
    }

    Common::Result<std::unique_ptr<sqlite3_stmt, void(*)(sqlite3_stmt*)> > DatabaseConnector::prepare_statement(const std::string& sql) {
        if (!is_connected()) {
            return Common::Result<std::unique_ptr<sqlite3_stmt, void(*)(sqlite3_stmt*)> >::Error(Common::StatusCode::DATABASE_ERROR, "数据库未连接");
        }

        sqlite3_stmt * raw_stmt = nullptr;
        int rc = sqlite3_prepare_v2(database_, sql.c_str(), -1, &raw_stmt, nullptr);

        if (rc != SQLITE_OK) {
            handle_sqlite_error(rc, "准备语句：" + sql);
            return Common::Result<std::unique_ptr<sqlite3_stmt, void(*)(sqlite3_stmt*)> >::Error(Common::StatusCode::DATABASE_ERROR, "准备语句失败");
        }

        auto stmt = std::unique_ptr<sqlite3_stmt, void(*)(sqlite3_stmt*)>(raw_stmt, [](sqlite3_stmt *s) { sqlite3_finalize(s); });

        return Common::Result<std::unique_ptr<sqlite3_stmt, void(*)(sqlite3_stmt*)> >::Success(std::move(stmt));
    }

    void DatabaseConnector::handle_sqlite_error(int error_code, const std::string& operation) {
        std::string error_msg = sqlite3_errmsg(database_);
        std::stringstream ss;
        ss << "SQL错误 [" << error_code << "]: " << error_msg << "（操作：" << operation <<"）";
        LOG_ERROR(ss.str());
    }

}