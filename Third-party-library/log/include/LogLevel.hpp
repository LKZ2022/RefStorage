//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

// include/log/LogLevel.hpp
#pragma once

#include<cstdint>

namespace Log
{

    // ��־����ö��
    enum class Level : uint8_t
    {
        LVL_TRACE = 0,  // ����ϸ�ĵ�����Ϣ
        LVL_DEBUG = 1,  // ����������Ϣ
        LVL_INFO = 2,  // ����������Ϣ
        LVL_WARN = 3,  // ������Ϣ
        LVL_ERROR = 4,  // ������Ϣ
        LVL_FATAL = 5   // ��������
    };

    // ����ת�ַ���
    const char* LevelToString(Level level);

    // ����ת���ַ���
    const char* LevelToShortString(Level level);

} // namespace Log
