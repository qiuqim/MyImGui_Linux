#pragma once
#include "imgui.h"
#include "imgui_internal.h"

namespace theme {
    // 颜色命名空间，方便组织不同类型的颜色
    namespace colors {
        // 基础颜色
        struct Base {
            ImColor accent;       // 主题色
            ImColor background;   // 主背景色
            ImColor separator;    // 分割线色
        };

        // 文本相关颜色
        struct Text {
            ImColor primary;      // 主要文本色
            ImColor secondary;    // 次要文本色
            ImColor active;       // 激活状态文本色
            ImColor hovered;      // 悬停状态文本色
            ImColor disabled;     // 禁用状态文本色
        };

        // 控件相关颜色
        struct Elements {
            ImColor background;           // 控件背景色
            ImColor background_hovered;   // 控件悬停背景色
            ImColor background_active;    // 控件激活背景色
            ImColor border;              // 控件边框色
            ImColor border_hovered;      // 控件悬停边框色
            ImColor border_active;       // 控件激活边框色
        };

        // 子窗口相关颜色
        struct Child {
            ImColor background;   // 子窗口背景色
            ImColor header;       // 子窗口标题栏色
            ImColor border;       // 子窗口边框色
        };

        // 页面相关颜色
        struct Page {
            ImColor background;           // 页面背景色
            ImColor background_active;    // 页面激活背景色
            ImColor text;                 // 页面文本色
            ImColor text_hovered;         // 页面文本悬停色
        };

        // 特殊控件颜色
        struct Special {
            ImColor checkbox_mark;        // 复选框标记色
            ImColor slider_grab;          // 滑块抓取色
            ImColor button_active;        // 按钮激活色
            ImColor notification_bg;      // 通知背景色
            ImColor tooltip_bg;           // 工具提示背景色
        };
    }

    // 主题配置结构体
    struct Theme {
        colors::Base base;
        colors::Text text;
        colors::Elements elements;
        colors::Child child;
        colors::Page page;
        colors::Special special;
    };

    // 预定义主题
    namespace presets {
        extern Theme light;  // 亮色主题
        extern Theme dark;   // 暗色主题
        // 可以在这里添加更多预设主题
    }

    // 当前主题颜色
    extern Theme current;

    // 颜色过渡配置
    struct TransitionConfig {
        float base_speed = 12.0f;        // 基础过渡速度
        float elements_speed = 25.0f;     // 控件过渡速度
        bool enable_transitions = true;   // 是否启用过渡动画
    };

    extern TransitionConfig transition_config;

    // 颜色插值函数
    inline ImColor LerpColor(const ImColor& a, const ImColor& b, float t) {
        return ImLerp(a.Value, b.Value, t);
    }

    // 更新主题颜色
    void UpdateTheme(bool is_dark, float delta_time);
} 