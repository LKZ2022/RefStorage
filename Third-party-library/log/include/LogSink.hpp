//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

// include/log/LogSink.hpp


#pragma once

#include "LogMessage.hpp"
#include <memory>

namespace Log
{

    // ǰ������
    class Formatter;

    // ���Ŀ��������
    class Sink
    {
    public:
        virtual ~Sink() = default;

        // д����־��Ϣ
        virtual void Write(const Message& msg) = 0;

        // ˢ�»�����
        virtual void Flush() = 0;

        // ����Ƿ�Ӧ�ü�¼�ü���
        virtual bool ShouldLog(Level level) const = 0;

        // ���ø�ʽ����
        virtual void SetFormatter(std::unique_ptr<Formatter> formatter) = 0;
    };

} // namespace Log
