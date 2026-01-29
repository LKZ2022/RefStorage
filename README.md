# RefStorage

一个基于现代C++实现的跨平台分布式文件存储系统，支持文件去重、分片存储、副本容错与并行传输。

**请注意：本项目目前处于积极开发阶段，核心功能正在构建中，尚未达到生产就绪状态。我们非常欢迎和期待社区的早期参与和贡献！**

## 📚 子仓库链接

### 🔗 直接访问
- **存储服务器端**: https://github.com/LKZ2022/storage_node

##  功能特性

-  **文件去重存储**：基于内容哈希消除重复数据，节省存储空间。
-  **分布式分片存储**：文件自动分片并分布存储在不同节点。
-  **多副本容错**：通过多副本机制确保数据高可用与持久性。
-  **并行上传下载**：充分利用带宽，提供高效的数据传输。
-  **RESTful API接口**：提供标准的HTTP API，便于集成。

##  架构设计

系统采用主从架构，职责清晰：

- **主服务器 (Master Server)**：负责全局元数据管理、存储调度与节点状态监控。
- **存储节点 (Storage Node)**：负责实际文件分片的存储、检索与本地管理。
- **客户端 (Client)**：提供简洁的上传、下载和管理接口，与主服务器交互。

##  开发进度

### 第一阶段 (进行中)
- [x] 项目基础结构与CMake构建系统搭建
- [x] 基础工具类（日志、配置、哈希）实现
- [x] 元数据数据库表结构初始化
- [ ] 第一阶段模块集成与基础测试

### 第二阶段 (规划中)
- [ ] 集中式存储核心逻辑（上传、下载、删除）
- [ ] 文件分片与去重处理器实现
- [ ] 简单的嵌入式HTTP API服务器

##  构建要求

要编译和构建本项目，请确保你的开发环境满足以下最低要求：

*   **编译器**: 支持 **C++20** 标准的编译器（如 GCC >= 11, Clang >= 13, MSVC >= 14.29）
*   **构建系统**: **CMake 3.5** 或更高版本
*   **项目依赖**: 本项目需要以下第三方库：
    *   **OpenSSL**: 用于网络通信加密、数据完整性校验（哈希计算）等安全功能。
    *   **SQLite3**: 用于存储系统的元数据（如文件索引、节点信息）。
    *   详细的安装与配置指南，请参阅 [依赖文档](./docs/DEPENDENCIES.md)。

##  开始上手

1.  **克隆所有仓库**
    ```bash
    # 克隆主仓库
    git clone https://github.com/LKZ2022/RefStorage.git
    cd RefStorage
    # 克隆存储服务器
    git clone https://github.com/LKZ2022/storage_node
    cd storage_node 
    ```

2.  **配置与构建**
    ```bash
    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build . --parallel
    ```
    *提示：顶层 `CMakeLists.txt` 中已通过 `set(CMAKE_CXX_STANDARD 20)` 强制要求C++20标准。*

3.  **运行测试** (功能开发中)
    ```bash
    ctest -V
    ```

##  如何贡献

我们深信，优秀的项目源于开放的协作。无论你是想报告Bug、提议新功能，还是直接提交代码，我们都无比欢迎！

1.  **发现Bug或有新点子？** 请先在 [Issue列表](https://github.com/LKZ2022/RefStorage/issues) 中查看是否已有相关讨论。如果没有，欢迎创建一个新的Issue。
2.  **想贡献代码？**
    *   Fork 本仓库。
    *   创建一个新的功能分支 (`git checkout -b feature/amazing-feature`)。
    *   提交你的更改 (`git commit -m ‘Add some amazing feature’`)。
    *   推送到分支 (`git push origin feature/amazing-feature`)。
    *   提交 Pull Request 并描述你的改动。

详细的贡献指南正在撰写中，欢迎直接通过Issue或Pull Request与我们交流！

##  许可证

本项目采用 **Apache License 2.0** 开源许可证。详情请见 [LICENSE](./LICENSE) 文件。

---

**感谢你的关注！期待与你一同构建未来的存储基础。**