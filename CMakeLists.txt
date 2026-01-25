#Copyright (c) 2026 Liu Kaizhi
#Licensed under the Apache License, Version 2.0.

cmake_minimum_required(VERSION 3.5)
project(RefStorage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/32516/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

link_directories(${PROJECT_SOURCE_DIR}/lib)

# 查找OpenSSL
find_package(OpenSSL REQUIRED)
# 查找SQLite3
find_package(SQLite3 REQUIRED)


add_library(logging STATIC
        Third-party-library/log/src/ColoredFormatter.cpp
        Third-party-library/log/src/ConsoleSink.cpp
        Third-party-library/log/src/FileSink.cpp
        Third-party-library/log/src/LogConfig.cpp
        Third-party-library/log/src/LogFormatter.cpp
        Third-party-library/log/src/Logger.cpp
        Third-party-library/log/src/LogLevel.cpp
)

target_include_directories(logging
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Third-party-library/log
        ${CMAKE_CURRENT_SOURCE_DIR}/Third-party-library/log/include
)

target_compile_definitions(logging
        PRIVATE
        -DLOG_SYSTEM_BUILD
)


#添加可执行文件
add_executable(${PROJECT_NAME} src/main.cpp
        src/core/metadata_manager/include/metadata_manager.hpp
        include/common/common_types.hpp
        src/utils/include/hash_utils.hpp
        src/utils/src/hash_utils.cpp
        src/utils/src/file_utils.cpp
        src/utils/include/file_utils.hpp
        src/database/src/database_connector.cpp
        src/database/include/database_connector.hpp
        src/database/include/database_config.hpp)


# 添加包含目录
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src/utils/include
)



target_link_libraries(${PROJECT_NAME}
        PRIVATE
        logging
        OpenSSL::SSL
        OpenSSL::Crypto
        SQLite::SQLite3
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/log/include)
