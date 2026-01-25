# 分布式文件存储系统 API 文档

## 1. 哈希工具类 (HashUtils)

### 概述
提供基于 OpenSSL 的哈希计算功能，主要用于文件去重和数据完整性验证。

### 主要类
```cpp
class HashUtils;  // 静态工具类，不可实例化
```

### 核心方法

#### 1.1 文件哈希计算
```cpp
static std::string calculateHash(const std::filesystem::path& path);
```
计算文件的 SHA256 哈希值。

**参数：**
- `filepath` - 文件路径

**返回：** 64位十六进制哈希字符串

**异常：** 文件不存在或读取失败时抛出 `std::runtime_error`

#### 1.2 哈希比较
```cpp
static bool hash_equals(const std::string& hash1, const std::string& hash2);
```
此处仅使用‘==’进行简单比较。


### 使用示例
```cpp
// 计算文件哈希
std::string hash = HashUtils::calculate_file_sha256("data.txt");

// 比较哈希
if (HashUtils::isEqualHash(hash1, hash2)) {
    LOG_INFO("文件相同");
}
```

---

## 2. 日志系统 (Logger)

### 概述
多级别、多输出的日志系统，支持控制台和文件输出，支持格式化字符串。

### 主要类
```cpp
namespace Log {
    class Logger;      // 日志主类（单例）
    class ConsoleSink; // 控制台输出
    class FileSink;    // 文件输出
    struct Config;     // 配置结构体
}
```

### 日志级别
```cpp
enum Level {
    LVL_TRACE,  // 跟踪信息
    LVL_DEBUG,  // 调试信息
    LVL_INFO,   // 普通信息
    LVL_WARN,   // 警告信息
    LVL_ERROR,  // 错误信息
    LVL_FATAL   // 致命错误
};
```

### 核心方法

#### 2.1 日志宏
```cpp
// 基础日志宏
LOG_TRACE("消息");      // 跟踪级别
LOG_DEBUG("消息");      // 调试级别  
LOG_INFO("消息");       // 信息级别
LOG_WARN("消息");       // 警告级别
LOG_ERROR("消息");      // 错误级别
LOG_FATAL("消息");      // 致命级别

// 格式化日志宏
LOG_INFO_FMT("用户 {0} 登录成功", username);
LOG_ERROR_FMT("连接失败: {0}, 错误码: {1}", reason, errno);
```
从0开始，依次增加。

#### 2.2 初始化配置
```cpp
// 快速初始化（最低级别）
logger.Initialize(Log::Level::LVL_DEBUG);
```

#### 2.3 输出目标管理
```cpp
// 添加控制台输出
logger.AddConsoleSink(
    Log::Level::LVL_DEBUG,                 // 最低输出级别
    true,                                  // 启用颜色
    "[%Y-%m-%d %H:%M:%S] [%l] %m"          // 输出格式
);

// 添加文件输出
logger.AddFileSink(
    "logs/system.log",                     // 日志文件路径
    Log::Level::LVL_INFO,                  // 最低输出级别
    "[%Y-%m-%d %H:%M:%S] [%l] [%f:%n] %m"  // 输出格式
);
```
输出格式可不填写.

### 格式字符串占位符
- `%Y` - 年份
- `%m` - 月份
- `%d` - 日期
- `%H` - 小时
- `%M` - 分钟
- `%S` - 秒
- `%l` - 日志级别
- `%f` - 文件名
- `%n` - 行号
- `%F` - 函数名
- `%m` - 消息内容
- `%%` - 百分号

### 使用示例
```cpp
// 初始化
auto& logger = Log::Logger::Instance();
logger.AddConsoleSink(Log::Level::LVL_DEBUG, true);
logger.AddFileSink("app.log", Log::Level::LVL_INFO);

// 记录日志
LOG_INFO("系统启动");
LOG_WARN_FMT("磁盘使用率: {}%", usage);
LOG_ERROR("数据库连接失败");
```

---

## 3. 数据库连接器 (DatabaseConnector) - 注意：未完成此模块

### 概述
SQLite 数据库连接和操作封装，支持预处理语句和事务。

### 主要类
```cpp
class DatabaseConnector;        // 数据库连接器
```

### 核心方法

#### 3.1 连接管理
```cpp
// 连接到数据库
bool connect(const std::string& database_path);
void disconnect();
bool is_connected() const;
```

#### 3.2 基本操作
```cpp
// 执行 SQL 语句
Result<bool> execute(const std::string& sql);

// 查询数据
template<typename T>
Result<std::vector<T>> query(
    const std::string& sql, 
    std::function<T(sqlite3_stmt*)> row_mapper
);
```

#### 3.3 预处理语句
```cpp
// 准备预处理语句
Result<PreparedStatementPtr> prepare_statement(const std::string& sql);

```


#### 3.5 事务管理
```cpp
Result<bool> begin_transaction();
Result<bool> commit_transaction();
Result<bool> rollback_transaction();
```

## 4. 公共类型定义 (Common Types)

### 概述
系统通用数据结构和类型别名。

### 主要类型

#### 4.1 基础类型别名
```cpp
using FileSize = uint64_t;                               // 文件大小
using Timestamp = std::chrono::system_clock::time_point; // 时间戳
using HashValue = std::string;                           // 哈希值
using NodeID = uint32_t;                                 // 节点ID
using ChunkID = uint64_t;                                // 分片ID
using FileID = uint64_t;                                 // 文件ID
```

#### 4.2 数据结构
```cpp
// 文件分片信息
struct ChunkInfo {
    ChunkID id;
    HashValue hash;
    FileSize size;
    uint32_t replica_count;
    std::vector<NodeID> storage_nodes;
    Timestamp create_time;
};

// 文件元数据
struct FileMetadata {
    FileID id;
    std::string filename;
    std::string path;
    FileSize size;
    HashValue hash;
    std::string mime_type;
    std::vector<ChunkInfo> chunks;
    uint64_t reference_count;                   // 引用计数（用于去重）
    Timestamp create_time;
    Timestamp last_access_time;
    
    bool is_deduplicated() const;               // 是否已去重
};

// 存储节点信息
struct StorageNode {
    NodeID id;
    std::string address;
    uint16_t port;
    FileSize total_capacity;
    FileSize used_capacity;
    bool is_active;
    Timestamp last_heartbeat;
    
    double usage_ratio() const;                 // 使用率
};
```

#### 4.3 操作结果
```cpp
// 状态码
enum class StatusCode {
    OK = 0,
    ERROR = 1,
    FILE_NOT_FOUND = 2,
    DUPLICATE_FILE = 3,
    STORAGE_FULL = 4,
    NETWORK_ERROR = 5,
    PERMISSION_DENIED = 6,
    INVALID_ARGUMENT = 7,
    DATABASE_ERROR = 8,
    SQL_ERROR = 9,
    STATEMENT_ERROR = 10
};

// 结果模板
template<typename T>
struct Result {
    StatusCode code;
    std::string message;
    T value;
    
    bool success() const;           // 是否成功
    bool failed() const;            // 是否失败
    T unwrap();                     // 获取值（失败时抛异常）
    T unwrap_or(T default_value);   // 获取值或默认值
    
    static Result<T> Success(T val);
    static Result<T> Error(StatusCode code, const std::string& msg);
};

// 特例：无返回值
template<>
struct Result<void>;

// 特例：预处理语句指针
template<>
struct Result<PreparedStatementPtr>;
```

---

## 5. 快速开始示例

```cpp
#include "utils/logger.h"
#include "utils/hash_utils.h"
#include "database/database_connector.h"
#include "database/database_initializer.h"

int main() {
    // 1. 初始化日志系统
    auto& logger = Log::Logger::Instance();
    logger.AddConsoleSink(Log::Level::LVL_DEBUG, true);
    logger.AddFileSink("system.log", Log::Level::LVL_INFO);
    LOG_INFO("系统启动");
    
    // 2. 计算文件哈希
    std::string hash = HashUtils::calculate_file_sha256("data.bin");
    LOG_INFO_FMT("文件哈希: {}", hash);
    
    // 3. 初始化数据库
    auto db = std::make_shared<DatabaseConnector>("dfs.db");
    DatabaseInitializer initializer(db);
    initializer.initialize_database();
    
    // 4. 使用预处理语句操作数据库
    auto stmt_result = db->prepare_statement(
        "INSERT INTO files (hash, filename, size) VALUES (?, ?, ?)"
    );
    
    if (stmt_result) {
        auto stmt = std::move(stmt_result.value);
        db->bind_text(stmt, 1, hash);
        db->bind_text(stmt, 2, "data.bin");
        db->bind_int64(stmt, 3, 1024);
        
        auto exec_result = db->execute_statement(stmt);
        if (exec_result.success()) {
            LOG_INFO("文件元数据保存成功");
        }
    }
    
    LOG_INFO("系统关闭");
    return 0;
}
```

---

## 6. 最佳实践

### 6.1 日志使用
1. **选择合适的级别**：
    - `DEBUG`：开发调试信息
    - `INFO`：正常操作信息
    - `WARN`：可恢复的问题
    - `ERROR`：需要关注的问题
    - `FATAL`：系统无法继续运行

2. **结构化日志**：
   ```cpp
   LOG_INFO_FMT("文件上传完成: 文件={}, 大小={}MB, 耗时={}ms", 
                filename, size_mb, duration_ms);
   ```

### 6.2 数据库操作
1. **总是使用预处理语句**防止SQL注入
2. **使用事务**保证数据一致性
3. **检查返回值**处理可能的错误

### 6.3 错误处理
1. **使用 Result 类型**统一错误处理
2. **记录错误日志**便于排查问题
3. **提供有意义的错误信息**

---

## 7. 故障排除

### 7.1 日志不输出
1. 检查日志级别设置
2. 确认初始化代码已执行
3. 检查文件路径权限

### 7.2 数据库连接失败
1. 确认数据库文件路径正确
2. 检查文件读写权限
3. 查看是否有其他进程占用

### 7.3 哈希计算失败
1. 确认文件存在且有读取权限
2. 检查 OpenSSL 库是否正确安装
3. 验证文件大小是否过大导致内存不足

---

此文档为分布式文件存储系统的基础组件API参考，详细实现请参考源代码和注释。