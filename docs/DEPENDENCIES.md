# 项目依赖

本文档说明如何为 RefStorage 项目安装和配置必需的第三方依赖。

## 必需依赖

### 1. OpenSSL
*   **用途**：保障通信安全，并提供 SHA-256 等哈希算法用于文件去重。
*   **安装**：
    *   **Ubuntu/Debian**: `sudo apt-get install libssl-dev`
    *   **CentOS/RHEL**: `sudo yum install openssl-devel`
    *   **macOS (Homebrew)**: `brew install openssl`
    *   **Windows (vcpkg)**: `vcpkg install openssl`

### 2. SQLite3
*   **用途**：作为轻量级元数据存储引擎，记录文件、分片和节点的映射关系。
*   **安装**：
    *   **Ubuntu/Debian**: `sudo apt-get install libsqlite3-dev`
    *   **CentOS/RHEL**: `sudo yum install sqlite-devel`
    *   **macOS (Homebrew)**: `brew install sqlite`
    *   **Windows (vcpkg)**: `vcpkg install sqlite3`

## 在 CMake 中定位依赖

本项目 CMake 已配置为使用 `find_package` 查找这些库。请确保它们安装在系统的标准路径，或通过 `-DCMAKE_PREFIX_PATH=<your_path>` 参数指定自定义安装位置。