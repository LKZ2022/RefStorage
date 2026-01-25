//Copyright (c) 2026 Liu Kaizhi
//Licensed under the Apache License, Version 2.0.

// include/log/ColoredFormatter.hpp


#pragma once

#include "LogFormatter.hpp"
#include <string>
#include <vector>
#include <memory>

namespace Log {

    // ��ɫ��������
    struct ColorRegion {
        enum class Type {
            TIMESTAMP,
            LEVEL,
            THREAD_ID,
            LOCATION,
            MESSAGE,
            CUSTOM_TEXT
        };

        Type type;
        std::string custom_text;
        bool colorize{ true };

        ColorRegion(Type t, bool c = true) : type(t), colorize(c) {}
        ColorRegion(const std::string& text, bool c = true)
            : type(Type::CUSTOM_TEXT), custom_text(text), colorize(c) {
        }
    };

    class ColoredFormatter : public Formatter {
    public:
        ColoredFormatter();
        explicit ColoredFormatter(const std::string& pattern);

        // ���ø�ʽ����ɫ����
        void SetFormat(const std::string& pattern,
            const std::vector<ColorRegion>& regions);

        // ��ʽ����Ϣ������ɫ����
        std::string Format(const Message& msg) const override;

        // �����Ƿ�ʹ����ɫ
        void SetUseColors(bool use_colors) { use_colors_ = use_colors; }

        // ��ȡ��ɫ����
        static std::string GetColorCode(Level level);
        static std::string GetResetCode();

    private:
        // ��ɫ�����ʵ��
        std::string ApplyColoring(const Message& msg) const;

        // �������ͻ�ȡ�ı�
        std::string GetTextForRegion(const ColorRegion& region, const Message& msg) const;

    private:
        std::vector<ColorRegion> regions_;
        bool use_colors_{ true };
    };

} // namespace Log
