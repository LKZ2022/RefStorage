//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

#pragma once
#include <string>

namespace RefStorage::DataBase {

    struct DatabaseConfig {
        std::string type_;                               // "postgresql", "mysql", "sqlite"
        std::string host_;
        int         port_;
        std::string database_;
        std::string username_;
        std::string password_;
        int         pool_size_  = 10;
        int         timeout_    = 30;

        // PostgreSQL 特定配置
        std::string schema      = "public";
        bool        use_ssl     = false;

        // SQLite 特定配置
        std::string db_file;
        bool        memory_mode = false;
    };

}
