//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

//	include/log/LogFormatter.hpp


#pragma once

#include "LogMessage.hpp"
#include <string>
#include <vector>
#include <map>

namespace Log {

    // ��ʽ��ģʽռλ��
    enum class FormatToken {
        TIMESTAMP,    // %t ʱ���
        LEVEL,        // %l ��־����
        LEVEL_SHORT,  // %L ��־���𣨶̣�
        FILE,         // %f �ļ���
        LINE,         // %n �к�
        FUNCTION,     // %F ������
        THREAD_ID,    // %T �߳�ID
        MESSAGE,      // %m ��Ϣ����
        PERCENT,      // %% �ٷֺ�
        TEXT          // ��ͨ�ı�
    };

    struct FormatPart {
        FormatToken token;
        std::string text;

        FormatPart(FormatToken t, const std::string& txt = "")
            : token(t), text(txt) {
        }
    };

    class Formatter {
    public:
        Formatter();
        explicit Formatter(const std::string& pattern);
        virtual ~Formatter() = default;

        // ���ø�ʽ
        virtual void SetPattern(const std::string& pattern);

        // ��ʽ����Ϣ
        virtual std::string Format(const Message& msg) const;

        // ��ȡ��ǰģʽ
        std::string GetPattern() const { return pattern_; }

    protected:
        // ����ģʽ�ַ���
        void ParsePattern(const std::string& pattern);

        // �滻ռλ��
        virtual std::string ReplaceToken(FormatToken token, const Message& msg) const;

    protected:
        std::string pattern_;
        std::vector<FormatPart> parts_;
    };

} // namespace Log
