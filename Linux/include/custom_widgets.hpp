#pragma once

// 启用数学运算符重载
#define IMGUI_DEFINE_MATH_OPERATORS 

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "theme.hpp"
#include <map>
#include <vector>
#include <string>

using namespace ImGui;

// namespace font
// {
// 	extern ImFont* icomoon_page;
// }
namespace font
{
   extern ImFont* icomoon_logo_;
   extern ImFont* icomoon_page_;
   extern ImFont* inter_semibold_;
   extern ImFont* icon_notify_;
}
namespace ImGui
{
  extern ImU32  GetColorU32(const ImVec4& col, float alpha_mul);

}
namespace custom 
{

// 动画系统
namespace anim {
    constexpr float SPEED = 6.0f;  // 默认动画速度
    
    // 动画曲线函数
    inline float EaseOutExpo(float x) {
        return x == 1.0f ? 1.0f : 1.0f - powf(2.0f, -10.0f * x);
    }
    
    inline float EaseInOutQuad(float x) {
        return x < 0.5f ? 2.0f * x * x : 1.0f - powf(-2.0f * x + 2.0f, 2.0f) / 2.0f;
    }

    inline float EaseInOutSine(float x) {
        return -(cosf(IM_PI * x) - 1.0f) / 2.0f;
    }
    
    inline float EaseOutBack(float x) {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return 1.0f + c3 * powf(x - 1.0f, 3.0f) + c1 * powf(x - 1.0f, 2.0f);
    }

    inline float EaseOutBounce(float x) {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;
        if (x < 1.0f / d1) {
            return n1 * x * x;
        } else if (x < 2.0f / d1) {
            x -= 1.5f / d1;
            return n1 * x * x + 0.75f;
        } else if (x < 2.5f / d1) {
            x -= 2.25f / d1;
            return n1 * x * x + 0.9375f;
        } else {
            x -= 2.625f / d1;
            return n1 * x * x + 0.984375f;
        }
    }
} // namespace anim

// 主题配置
// namespace theme {
//     struct Colors {
//         ImVec4 accent = ImColor(118, 187, 117);          // 主题色
//         ImVec4 background = ImColor(15, 15, 15);     // 背景色
//         ImVec4 background_light{0.15f, 0.15f, 0.15f, 1.0f}; // 亮背景色
//         ImVec4 text{1.0f, 1.0f, 1.0f, 0.85f};            // 文本色
//         ImVec4 text_dim{1.0f, 1.0f, 1.0f, 0.65f};        // 暗文本色
//         ImVec4 border{0.43f, 0.43f, 0.50f, 0.50f};       // 边框色
//         ImVec4 slider_grab{0.24f, 0.52f, 0.88f, 1.0f};   // 滑块色
//         ImVec4 text_active{0.28f, 0.56f, 1.0f, 1.0f};     // 激活文本色
//         ImVec4 text_hov{1.0f, 1.0f, 1.0f, 0.85f};        // 悬停文本色
//         ImVec4 separator = ImColor(22, 23, 26);      // 分割线色
//         ImVec4 background_active = ImColor(31, 33, 38);
//     };

//     struct Metrics {
//         float rounding{6.0f};           // 圆角大小
//         float animation_speed{6.0f};    // 动画速度
//         float indent_spacing{21.0f};    // 缩进间距
//         float scrollbar_size{14.0f};    // 滚动条大小
//     };

//     extern Colors colors;
//     extern Metrics metrics;
// } // namespace theme

// 内部状态结构体
/* namespace internal {
    // 基础状态
    struct ElementState {
        ImVec4 background{theme::current.elements.background};
        ImVec4 text{theme::current.text.secondary};
        float alpha{0.0f};
        float scale{1.0f};
        bool active{false};
        bool hovered{false};
    };

    // 页面状态
    struct PageState : ElementState {
        float smooth_swap{0.0f};    // 切换动画进度
        float alpha_line{0.0f};     // 底部线条透明度
        float line_size{0.0f};      // 底部线条大小

    };

    // 按钮状态
    struct ButtonState : ElementState {
        float press_anim{0.0f};     // 按下动画
    };

    // 复选框状态
    struct CheckboxState : ElementState {
        float alpha_mark{0.0f};     // 勾选标记透明度
    };

    // 组合框状态
    struct ComboState : ElementState {
        float arrow_roll{0.0f};     // 箭头旋转角度
        bool opened{false};         // 下拉框状态
        float height{0.0f};         // 动画高度
        float combo_size{0.0f};    // 下拉框大小
    };

    // 多选组合框状态
    struct MultiComboState : ComboState {
        std::vector<bool> selected;
    };

    // 按键绑定状态
    struct KeybindState : ElementState {
        bool waiting{false};        // 等待输入状态
        float wait_anim{0.0f};     // 等待动画
    };

    // 滑动条状态
    struct SliderState : ElementState {
        float grab_pos{0.0f};       // 滑块位置
        bool dragging{false};       // 拖拽状态
        float grab_anim{0.0f};      // 滑块动画
        float slow{0.0f};          // 滑动条位置
    };

    // 颜色选择器状态
    struct ColorPickerState : ElementState {
        float hue_cursor{0.0f};     // 色相光标
        float sat_cursor{0.0f};     // 饱和度光标
        float val_cursor{0.0f};     // 明度光标
        float alpha_cursor{1.0f};   // 透明度光标
        bool dragging_hue{false};   // 拖拽色相
        bool dragging_sv{false};    // 拖拽饱和度/明度
        float hue_bar{0.0f};         // 色相光标
        float alpha_bar{0.0f};      // 透明度光标
        float circle{0.0f};        // 圆圈
        ImVec2 circle_move{0.0f, 0.0f}; // 圆圈移动
    };

    // 旋钮状态
    struct KnobState : ElementState {
        float angle{0.0f};          // 当前角度
        float value{0.0f};          // 当前值
        bool dragging{false};       // 拖拽状态
        float drag_angle{0.0f};     // 拖拽角度
        float plus_float{0.0f};    // 增加值
        int plus_int{0};           // 增加值
        float circle_anim{0.0f};   // 圆圈动画
        float position{0.0f};      // 位置
    };

    struct theme_state : ElementState {
        float smooth_swap;          // 切换动画进度
        float alpha_line;           // 底部线条透明度
        float line_size;            // 底部线条大小
    };

    // 子窗口状态
    struct ChildState : ElementState {
        bool capped{false};         // 是否有标题
        float header_alpha{0.0f};   // 标题透明度
        float scroll_alpha{0.0f};   // 滚动条透明度
    };

    // 颜色编辑状态
    struct ColorEditState : ElementState {
        float popup_anim{0.0f};     // 弹出动画
    };
} // namespace internal
 */

// 控件状态管理
namespace internal {
     //圆角
    static float rounding{6.0f};
    // 基础状态
    struct ElementState {
       
        // 颜色状态
        struct Colors {
            ImVec4 background;    // 背景色
            ImVec4 text;         // 文本色
            ImVec4 border;       // 边框色
            ImVec4 accent;       // 强调色
        } colors;

        // 动画状态
        struct Animation {
            float alpha{0.0f};      // 透明度
            float scale{1.0f};      // 缩放
        } anim;

        // 交互状态
        struct Interaction {
            bool active{false};     // 激活状态
            bool hovered{false};    // 悬停状态
            bool focused{false};    // 焦点状态
            bool disabled{false};   // 禁用状态
        } state;

        // 构造函数：使用主题系统初始化默认颜色
        ElementState() {
            colors.background = theme::current.elements.background;
            colors.text = theme::current.text.secondary;
            colors.border = theme::current.elements.border;
            colors.accent = theme::current.base.accent;
        }

        // 更新颜色状态
        virtual void UpdateColors(float delta_time) {
            const float speed = anim::SPEED;
            
            // 根据状态更新颜色
            ImVec4 target_bg = state.disabled ? theme::current.elements.background 
                : state.active ? theme::current.elements.background_active
                : state.hovered ? theme::current.elements.background_hovered
                : theme::current.elements.background;

            ImVec4 target_text = state.disabled ? theme::current.text.disabled
                : state.active ? theme::current.text.active
                : state.hovered ? theme::current.text.hovered
                : theme::current.text.secondary;

            // 应用颜色过渡
            colors.background = ImLerp(colors.background, target_bg, delta_time * speed);
            colors.text = ImLerp(colors.text, target_text, delta_time * speed);
            colors.border = ImLerp(colors.border, 
                state.focused ? theme::current.elements.border_active : theme::current.elements.border, 
                delta_time * speed);
            colors.accent = ImLerp(colors.accent, theme::current.base.accent, delta_time * speed);
        }
    };

    // 按钮状态
    struct ButtonState : ElementState {
        float press_anim{0.0f};
        float hover_anim{0.0f};
        void UpdateColors(float delta_time) override {
            ElementState::UpdateColors(delta_time);
            // 按钮特有的颜色逻辑
            if (state.active) {
                colors.background = ImLerp(colors.background, theme::current.special.button_active, delta_time * anim::SPEED);
            }
        }
    };

    // 页面状态
    struct PageState : ElementState {
        struct {
            float swap{0.0f};       // 切换动画
            float line_alpha{0.0f}; // 线条透明度
            float line_size{0.0f};  // 线条大小
            float smooth_swap{0.0f}; // 平滑切换动画
        } page_anim;

        void UpdateColors(float delta_time) override {
            ElementState::UpdateColors(delta_time);
            // 背景颜色插值: 根据状态在背景色和主题色之间过渡
            colors.background = ImLerp(colors.background, 
                state.active || state.hovered ? theme::current.page.background_active : theme::current.page.background, 
                delta_time * anim::SPEED);
                // 文本颜色插值: 根据状态调整透明度
            colors.text = ImLerp(colors.text,
                state.active ? theme::current.base.accent : state.hovered ? theme::current.page.text_hovered : theme::current.page.text,
                delta_time * 14.0f);
        }
    };

    // 复选框状态
    struct CheckboxState : ElementState {
        float mark_alpha{0.0f};

        void UpdateColors(float delta_time) override {
            ElementState::UpdateColors(delta_time);
            if (state.active) {
                colors.background = ImLerp(colors.background, theme::current.base.accent, delta_time * anim::SPEED);
                colors.text = ImLerp(colors.text, theme::current.special.checkbox_mark, delta_time * anim::SPEED);
            }
        }
    };

    // 滑动条状态
    struct SliderState : ElementState {
        struct {
            float pos{0.0f};        // 滑块位置
            float anim{0.0f};       // 滑块动画
            float slow{0.0f};       // 缓动值
        } slider;
        bool dragging{false};

        void UpdateColors(float delta_time) override {
            ElementState::UpdateColors(delta_time);
            if (dragging) {
                colors.accent = ImLerp(colors.accent, theme::current.special.slider_grab, delta_time * anim::SPEED);
            }
        }
    };

    // 子窗口状态
    struct ChildState : ElementState {
        struct {
            bool has_cap{false};
            float header_alpha{0.0f};
            float scroll_alpha{0.0f};
        } child;

        void UpdateColors(float delta_time) override {
            ElementState::UpdateColors(delta_time);
            colors.background = ImLerp(colors.background, theme::current.child.background, delta_time * anim::SPEED);
            if (child.has_cap) {
                colors.accent = ImLerp(colors.accent, theme::current.child.header, delta_time * anim::SPEED);
            }
        }
    };

    // 组合框状态
    struct ComboState : ElementState {
        float arrow_roll{0.0f};//箭头旋转角度
        bool opened{false};//下拉框状态
        float height{0.0f};//动画高度
        float combo_size{0.0f};//下拉框大小
    };

    // Switch开关状态
    struct SwitchState : ElementState {
        bool active{false};//激活状态
        float switch_anim{0.0f};//开关动画
        float last_anim{0.0f};//上次动画
    };

    // 颜色选择器状态
    struct ColorPickerState : ElementState {
        float hue_cursor{0.0f};//色相光标
        float sat_cursor{0.0f};//饱和度光标
        float val_cursor{0.0f};//明度光标
        float alpha_cursor{1.0f};//透明度光标
        bool dragging_hue{false};//拖拽色相
        bool dragging_sv{false};//拖拽饱和度/明度
        float hue_bar{0.0f};//色相光标
        float alpha_bar{0.0f};//透明度光标
        float circle{0.0f};//圆圈
        ImVec2 circle_move{0.0f, 0.0f};//圆圈移动
    };

    // 旋钮状态
    struct KnobState : ElementState {
        float angle{0.0f};//当前角度
        float value{0.0f};//当前值
        bool dragging{false};//拖拽状态
        float drag_angle{0.0f};//拖拽角度
        float plus_float{0.0f};//增加值
        int plus_int{0};//增加值
        float circle_anim{0.0f};//圆圈动画
        float position{0.0f};//位置
    };

    // 主题状态
    struct theme_state : ElementState {
        float smooth_swap;
        float alpha_line;
        float line_size;
    };

    // 列表项状态
    struct ListItemState : ElementState {
        float hover_anim{0.0f};
    };

} // namespace internal


// 控件颜色管理
namespace style {
    // 获取控件颜色，统一管理颜色获取逻辑
    inline ImU32 GetColor(const ImVec4& color, float alpha_mul = 1.0f) {
        return ImGui::GetColorU32(color, alpha_mul);
    }

    // 获取主题颜色
    inline ImU32 GetThemeColor(const ImVec4& color) {
        return GetColor(color);
    }

    // 获取状态颜色
    template<typename T>
    inline ImU32 GetStateColor(const T& state, float alpha_mul = 1.0f) {
        return GetColor(state.colors.background, alpha_mul);
    }

    // 获取文本颜色
    template<typename T>
    inline ImU32 GetTextColor(const T& state, float alpha_mul = 1.0f) {
        return GetColor(state.colors.text, alpha_mul);
    }
} // namespace style

// 工具函数
namespace utils {
    inline ImVec2 ImVec2Add(const ImVec2& a, const ImVec2& b) {
        return ImVec2(a.x + b.x, a.y + b.y);
    }

    inline ImVec2 ImVec2Sub(const ImVec2& a, const ImVec2& b) {
        return ImVec2(a.x - b.x, a.y - b.y);
    }

    inline ImVec2 ImVec2Mul(const ImVec2& a, const float& b) {
        return ImVec2(a.x * b, a.y * b);
    }

    inline ImVec4 ColorWithAlpha(const ImVec4& color, float alpha) {
        return ImVec4(color.x, color.y, color.z, alpha);
    }
    
    inline float Lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }
    
    inline ImVec4 Lerp(const ImVec4& a, const ImVec4& b, float t) {
        return ImVec4(
            Lerp(a.x, b.x, t),
            Lerp(a.y, b.y, t),
            Lerp(a.z, b.z, t),
            Lerp(a.w, b.w, t)
        );
    }

    inline float GetKnobAngle(float value, float v_min, float v_max) {
        return (value - v_min) / (v_max - v_min) * IM_PI * 1.5f - IM_PI * 0.75f;
    }

    inline float GetKnobValue(float angle, float v_min, float v_max) {
        return v_min + (angle + IM_PI * 0.75f) / (IM_PI * 1.5f) * (v_max - v_min);
    }

    inline ImVec4 GetRainbowColor(float t) {
        const float r = 0.5f + 0.5f * cosf(t * 6.28318f);
        const float g = 0.5f + 0.5f * cosf(t * 6.28318f + 2.09439f);
        const float b = 0.5f + 0.5f * cosf(t * 6.28318f + 4.18879f);
        return ImVec4(r, g, b, 1.0f);
    }
} // namespace utils

// 控件声明
// 基础控件
bool Page(bool selected, const char* icon, const ImVec2& size_arg);
bool Button(const char* label, const ImVec2& size_arg);
bool ThemeButton(const char* id_theme, bool dark, const ImVec2& size_arg);
bool Checkbox(const char* label, bool* v);
bool CheckboxClicked(const char* label, bool* v);
// Switch控件声明
bool Switch1(const char* label, bool* v);
bool Switch(const char* label, bool* v);
// 选择控件
bool Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

// 组合框控件
bool BeginCombo(const char* label, const char* preview_value, int val = 0, bool multi = false, ImGuiComboFlags flags = 0);
void EndCombo();
void MultiCombo(const char* label, bool variable[], const char* labels[], int count);
bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
bool Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1);

// 颜色控件
bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);

// 数值控件
// bool KnobScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
// bool KnobFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
// bool KnobInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);

bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);


// 子窗口
bool ChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool cap = false, ImGuiWindowFlags flags = 0);
bool Child(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool cap = false, ImGuiWindowFlags flags = 0);
bool ChildID(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool cap = false, ImGuiWindowFlags flags = 0);
void EndChild();

// 布局控件
void BeginGroup();
void EndGroup();
void Separator();
void Separator_line();
void SeparatorEx(ImGuiSeparatorFlags flags, float thickness = 1.0f);





// M3 风格控件声明
namespace m3 {
    // 按钮类型
    enum class ButtonType {
        Filled,      // 填充按钮
        Outlined,    // 轮廓按钮
        Text,        // 文本按钮
        Elevated,    // 浮起按钮
        Tonal       // 色调按钮
    };

    // 按钮组件
    bool Button(const char* label, ButtonType type = ButtonType::Filled, const ImVec2& size = ImVec2(0, 0));
    
    // FAB (浮动操作按钮)
    bool FAB(const char* icon, const char* tooltip = nullptr, bool small = false);
    
    // 卡片
    void BeginCard(const char* str_id, bool elevated = false);
    void EndCard();
    
    // 分割线
    void Divider(bool vertical = false);
    
    // 列表项
    bool ListItem(const char* label, const char* description = nullptr, const char* icon = nullptr);
    
    // 导航抽屉项
    bool NavItem(const char* label, const char* icon, bool selected = false);
    
    // 复选框
    bool Checkbox(const char* label, bool* v);
    
    // 单选按钮
    bool RadioButton(const char* label, int* v, int v_button);
    
    // 滑块
    bool Slider(const char* label, float* v, float v_min, float v_max);
    
    // 进度指示器
    void ProgressBar(float fraction, const ImVec2& size = ImVec2(-FLT_MIN, 0));
    void LoadingIndicator(const ImVec2& size = ImVec2(0, 0));
    
    // 芯片
    bool Chip(const char* label, bool* selected = nullptr, bool closable = false);
    
    // 文本输入框
    bool InputText(const char* label, char* buf, size_t buf_size);
    
    // 下拉菜单
    bool Dropdown(const char* label, int* current_item, const char* const items[], int items_count);
}

// 图标定义
namespace icons {
    constexpr const char* ADD           = "+";//"";      // 添加图标
    constexpr const char* REMOVE        = "-";//"";      // 删除图标
    constexpr const char* SETTINGS      = "S";//"";      // 设置图标
    constexpr const char* USER          = "U";//"";      // 用户图标
    constexpr const char* HOME          = "H";//"";      // 主页图标
    constexpr const char* SEARCH        = "F";//"";      // 搜索图标
    constexpr const char* MENU          = "M";//"";      // 菜单图标
    constexpr const char* CLOSE         = "X";//"";      // 关闭图标
    constexpr const char* CHECK         = "✓";//"";      // 勾选图标
    constexpr const char* WARNING       = "!";//"";      // 警告图标
    constexpr const char* INFO          = "i";//"";      // 信息图标
    constexpr const char* ERROR         = "E";//"";      // 错误图标
    constexpr const char* SUCCESS       = "O";//"";      // 成功图标
    // 可以继续添加更多图标...
}

// 进度条
void ProgressBar(float fraction, const ImVec2& size = ImVec2(-FLT_MIN, 0));

// 加载指示器
void LoadingSpinner(const char* label, float radius, float thickness, const ImU32& color);

// 标签页
bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0);
bool TabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0);

// 工具提示
void Tooltip(const char* text);

// 可折叠标题
bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);

}//namespace custom

