--Copyright (c) 2026 Liu Kaizhi
--Licensed under the Apache License, Version 2.0.

-- 文件元数据表
CREATE TABLE IF NOT EXISTS files (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    hash CHAR(64) UNIQUE NOT NULL,
    filename VARCHAR(255) NOT NULL,
    path VARCHAR(1024),
    size BIGINT NOT NULL,
    mime_type VARCHAR(100) COMMIT 'MIME类型',
    reference_count INTEGER DEFAULT 1,
    deduplication_enabled BOOLEAN DEFAULT 1,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_accessed_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    metadata TEXT COMMIT '额外的元数据（Json格式）',

    INDEX idx_hash (hash),
    INDEX idx_filename (filename),
    INDEX idx_created_at (created_at),
    INDEX idx_reference_count (reference_count)
);

