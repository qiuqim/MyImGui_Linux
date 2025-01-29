#include "custom_widgets.hpp"
#include <map>
#include <iostream>
#include <functional>

//m3
namespace m3 {

bool Button(const char* label, ButtonType type, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, 
                              label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // 状态管理
    static std::map<ImGuiID, internal::ButtonState> states;
    auto state = states.find(id);
    if (state == states.end())
    {
        states.insert({id, internal::ButtonState()});
        state = states.find(id);
    }

    // 更新状态动画
    const float target_anim = (hovered || held) ? 1.0f : 0.0f;
    state->second.hover_anim = ImLerp(
        state->second.hover_anim,
        target_anim,
        g.IO.DeltaTime * anim::SPEED);

    // 渲染
    ImDrawList* draw_list = window->DrawList;
    const float rounding = size.y * 0.5f; // M3风格的圆角

    switch (type)
    {
        case ButtonType::Filled:
        {
            // 背景
            ImU32 bg_col = GetColorU32(
                ImLerp(
                    ImVec4(theme::current.base.accent),
                    ImVec4(theme::current.elements.background_active),
                    state->second.hover_anim
                )
            );
            
            draw_list->AddRectFilled(bb.Min, bb.Max, bg_col, rounding);
            
            // 文本
            PushStyleColor(ImGuiCol_Text, theme::current.special.checkbox_mark.Value);
            RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, 
                            label, NULL, &label_size, style.ButtonTextAlign, &bb);
            PopStyleColor();
            break;
        }
        
        case ButtonType::Outlined:
        {
            // 边框
            draw_list->AddRect(bb.Min, bb.Max, 
                             GetColorU32(theme::current.elements.border), rounding);
            
            // 悬停效果
            if (state->second.hover_anim > 0.0f)
            {
                draw_list->AddRectFilled(bb.Min, bb.Max,
                    GetColorU32(ImVec4(
                        theme::current.elements.background_hovered.Value.x,
                        theme::current.elements.background_hovered.Value.y,
                        theme::current.elements.background_hovered.Value.z,
                        state->second.hover_anim * 0.15f
                    )), rounding);
            }
            
            // 文本
            RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding,
                            label, NULL, &label_size, style.ButtonTextAlign, &bb);
            break;
        }
        
        // ... 其他按钮类型的实现 ...
    }

    return pressed;
}



void BeginCard(const char* str_id, bool elevated)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f); // M3 风格的圆角
    PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    
    ImGuiID id = window->GetID(str_id);
    bool open = BeginChild(id, ImVec2(0, 0), false, 
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    if (open)
    {
        // 添加阴影或高亮效果
        if (elevated)
        {
            ImDrawList* draw_list = GetWindowDrawList();
            ImRect bb = GetCurrentWindow()->Rect();
            draw_list->AddRectFilled(
                bb.Min + ImVec2(0, 2),
                bb.Max + ImVec2(0, 2),
                IM_COL32(0, 0, 0, 20),
                12.0f
            );
        }
    }
}

void EndCard()
{
    EndChild();
    PopStyleVar(2);
}
} // namespace m3
} // namespace custom



namespace m3 {

// FAB (浮动操作按钮)
bool FAB(const char* icon, const char* tooltip, bool small)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(icon);
    
    // FAB 尺寸
    const float size = small ? 40.0f : 56.0f;
    const float icon_size = small ? 24.0f : 32.0f;
    
    ImVec2 pos = window->DC.CursorPos;
    const ImRect bb(pos, pos + ImVec2(size, size));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // 状态管理
    static std::map<ImGuiID, internal::ButtonState> states;
    auto state = states.find(id);
    if (state == states.end())
    {
        states.insert({id, internal::ButtonState()});
        state = states.find(id);
    }

    // 更新动画
    const float target_anim = (hovered || held) ? 1.0f : 0.0f;
    state->second.hover_anim = ImLerp(
        state->second.hover_anim,
        target_anim,
        g.IO.DeltaTime * anim::SPEED);

    // 渲染
    ImDrawList* draw_list = window->DrawList;
    
    // 阴影效果
    draw_list->AddCircleFilled(
        bb.GetCenter() + ImVec2(0, 2),
        size * 0.5f,
        IM_COL32(0, 0, 0, 32)
    );
    
    // 背景
    ImU32 bg_col = GetColorU32(
        ImLerp(
            ImVec4(theme::current.base.accent),
            ImVec4(theme::current.elements.background_active),
            state->second.hover_anim
        )
    );
    draw_list->AddCircleFilled(bb.GetCenter(), size * 0.5f, bg_col);

    // 图标
    const ImVec2 icon_pos = bb.GetCenter() - ImVec2(icon_size * 0.5f, icon_size * 0.5f);
    PushFont(font::icomoon_logo_);
    RenderText(icon_pos, icon);
    PopFont();

    // 工具提示
    if (tooltip && hovered)
        SetTooltip("%s", tooltip);

    return pressed;
}

// 分割线
void Divider(bool vertical)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    
    if (vertical)
    {
        float x = window->DC.CursorPos.x;
        float h = window->WorkRect.GetHeight();
        
        window->DrawList->AddLine(
            ImVec2(x, window->DC.CursorPos.y),
            ImVec2(x, window->DC.CursorPos.y + h),
            GetColorU32(theme::current.separator.separator.Value),
            1.0f
        );
        
        ItemSize(ImVec2(1.0f, h));
    }
    else
    {
        float w = window->WorkRect.GetWidth();
        
        window->DrawList->AddLine(
            window->DC.CursorPos,
            window->DC.CursorPos + ImVec2(w, 0.0f),
            GetColorU32(theme::current.separator.separator.Value),
            1.0f
        );
        
        ItemSize(ImVec2(0.0f, 1.0f));
    }
}

// 列表项
bool ListItem(const char* label, const char* description, const char* icon)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const float icon_size = 24.0f;
    const float height = description ? 72.0f : 56.0f;
    const float text_offset_x = icon ? (icon_size + style.ItemSpacing.x) : 0.0f;
    
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size(window->WorkRect.GetWidth(), height);
    
    const ImRect bb(pos, pos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // 状态管理
    static std::map<ImGuiID, internal::ListItemState> states;
    auto state = states.find(id);
    if (state == states.end())
    {
        states.insert({id, internal::ListItemState()});
        state = states.find(id);
    }

    // 更新动画
    const float target_anim = (hovered || held) ? 1.0f : 0.0f;
    state->second.hover_anim = ImLerp(
        state->second.hover_anim,
        target_anim,
        g.IO.DeltaTime * anim::SPEED);

    // 渲染
    ImDrawList* draw_list = window->DrawList;
    
    // 背景
    if (state->second.hover_anim > 0.0f)
    {
        draw_list->AddRectFilled(
            bb.Min, bb.Max,
            GetColorU32(ImVec4(
                theme::current.elements.background_hovered.Value.x,
                theme::current.elements.background_hovered.Value.y,
                theme::current.elements.background_hovered.Value.z,
                state->second.hover_anim * 0.15f
            )),
            4.0f
        );
    }

    // 图标
    if (icon)
    {
        PushFont(font::icomoon_page_);
        RenderText(
            ImVec2(bb.Min.x + style.FramePadding.x, 
                   bb.Min.y + (height - icon_size) * 0.5f),
            icon
        );
        PopFont();
    }

    // 主标签
    RenderText(
        ImVec2(bb.Min.x + text_offset_x + style.FramePadding.x,
               description ? (bb.Min.y + 16.0f) : (bb.Min.y + (height - g.FontSize) * 0.5f)),
        label
    );

    // 描述文本
    if (description)
    {
        PushStyleColor(ImGuiCol_Text, theme::current.text.secondary.Value);
        RenderText(
            ImVec2(bb.Min.x + text_offset_x + style.FramePadding.x,
                   bb.Min.y + 40.0f),
            description
        );
        PopStyleColor();
    }

    return pressed;
}

// 复选框
bool Checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    // 状态管理
    static std::map<ImGuiID, internal::ButtonState> states;
    auto state = states.find(id);
    if (state == states.end())
    {
        states.insert({id, internal::ButtonState()});
        state = states.find(id);
    }

    // 更新动画
    const float target_anim = *v ? 1.0f : 0.0f;
    state->second.hover_anim = ImLerp(
        state->second.hover_anim,
        (hovered || held) ? 1.0f : 0.0f,
        g.IO.DeltaTime * anim::SPEED);
    
    state->second.active_anim = ImLerp(
        state->second.active_anim,
        target_anim,
        g.IO.DeltaTime * anim::SPEED);

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    
    // 渲染
    ImDrawList* draw_list = window->DrawList;
    const float rounding = square_sz * 0.25f;

    // 背景动画
    const ImU32 bg_col = GetColorU32(
        ImLerp(
            ImVec4(theme::current.elements.background),
            ImVec4(theme::current.base.accent),
            state->second.active_anim
        )
    );

    // 绘制背景
    draw_list->AddRectFilled(check_bb.Min, check_bb.Max, bg_col, rounding);

    // 绘制边框
    if (state->second.active_anim < 1.0f)
    {
        draw_list->AddRect(
            check_bb.Min, check_bb.Max,
            GetColorU32(theme::current.elements.border),
            rounding
        );
    }

    // 绘制勾选标记
    if (state->second.active_anim > 0.0f)
    {
        const float check_sz = square_sz * 0.5f;
        const ImVec2 center = check_bb.GetCenter();
        const float thickness = ImMax(square_sz * 0.1f, 1.0f);
        
        draw_list->AddLine(
            ImVec2(center.x - check_sz * 0.25f, center.y),
            ImVec2(center.x - check_sz * 0.1f, center.y + check_sz * 0.3f),
            GetColorU32(theme::current.special.checkbox_mark),
            thickness
        );
        draw_list->AddLine(
            ImVec2(center.x - check_sz * 0.1f, center.y + check_sz * 0.3f),
            ImVec2(center.x + check_sz * 0.4f, center.y - check_sz * 0.3f),
            GetColorU32(theme::current.special.checkbox_mark),
            thickness
        );
    }

    // 绘制标签
    if (label_size.x > 0.0f)
    {
        RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), label);
    }

    return pressed;
}

// 单选按钮
bool RadioButton(const char* label, int* v, int v_button)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = v_button;
        MarkItemEdited(id);
    }

    // 状态管理
    static std::map<ImGuiID, internal::ElementState> states;
    auto state = states.find(id);
    if (state == states.end())
    {
        states.insert({id, internal::ElementState()});
        state = states.find(id);
    }

    // 更新动画
    const float target_anim = (*v == v_button) ? 1.0f : 0.0f;
    state->second.hover_anim = ImLerp(
        state->second.hover_anim,
        (hovered || held) ? 1.0f : 0.0f,
        g.IO.DeltaTime * anim::SPEED);
    
    state->second.active_anim = ImLerp(
        state->second.active_anim,
        target_anim,
        g.IO.DeltaTime * anim::SPEED);

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    
    // 渲染
    ImDrawList* draw_list = window->DrawList;

    // 外圈
    draw_list->AddCircle(
        check_bb.GetCenter(),
        square_sz * 0.5f,
        GetColorU32(theme::current.elements.border),
        0,
        2.0f
    );

    // 选中状态的填充
    if (state->second.active_anim > 0.0f)
    {
        const float radius = square_sz * 0.25f * state->second.active_anim;
        draw_list->AddCircleFilled(
            check_bb.GetCenter(),
            radius,
            GetColorU32(theme::current.base.accent)
        );
    }

    // 绘制标签
    if (label_size.x > 0.0f)
    {
        RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), label);
    }

    return pressed;
}

// 进度条
void ProgressBar(float fraction, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
    ImRect bb(pos, pos + size);
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return;

    // 渲染
    ImDrawList* draw_list = window->DrawList;
    const float rounding = size.y * 0.5f;

    // 背景
    draw_list->AddRectFilled(
        bb.Min,
        bb.Max,
        GetColorU32(theme::current.elements.background),
        rounding
    );

    // 进度
    if (fraction > 0.0f)
    {
        float progress_width = ImLerp(0.0f, size.x, fraction);
        draw_list->AddRectFilled(
            bb.Min,
            ImVec2(bb.Min.x + progress_width, bb.Max.y),
            GetColorU32(theme::current.base.accent),
            rounding,
            progress_width < rounding ? ImDrawFlags_RoundCornersLeft : ImDrawFlags_RoundCornersAll
        );
    }
}

// 滑块控件
bool Slider(const char* label, float* v, float v_min, float v_max)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    
    const float slider_height = 4.0f;  // M3风格的滑块高度
    const float thumb_radius = 10.0f;  // 滑块圆点半径
    const float grab_padding = 20.0f;  // 扩大可点击区域

    const ImVec2 pos = window->DC.CursorPos;
    const ImVec2 size(w, thumb_radius * 2.0f);
    const ImRect bb(pos, pos + size);
    
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    // 交互逻辑
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    if (held)
    {
        const float mouse_pos_x = g.IO.MousePos.x - bb.Min.x;
        float new_value = ImClamp((mouse_pos_x / w) * (v_max - v_min) + v_min, v_min, v_max);
        if (*v != new_value)
        {
            *v = new_value;
            MarkItemEdited(id);
        }
    }

    // 状态管理
    static std::map<ImGuiID, internal::ElementState> states;
    auto state = states.find(id);
    if (state == states.end())
    {
        states.insert({id, internal::ElementState()});
        state = states.find(id);
    }

    // 更新动画
    state->second.hover_anim = ImLerp(
        state->second.hover_anim,
        (hovered || held) ? 1.0f : 0.0f,
        g.IO.DeltaTime * anim::SPEED);

    // 渲染
    ImDrawList* draw_list = window->DrawList;
    const float value_x = ImLerp(bb.Min.x, bb.Max.x, (*v - v_min) / (v_max - v_min));
    const float center_y = bb.Min.y + size.y * 0.5f;

    // 背景轨道
    draw_list->AddRectFilled(
        ImVec2(bb.Min.x, center_y - slider_height * 0.5f),
        ImVec2(bb.Max.x, center_y + slider_height * 0.5f),
        GetColorU32(theme::current.elements.background),
        slider_height * 0.5f
    );

    // 已选择部分
    if (value_x > bb.Min.x)
    {
        draw_list->AddRectFilled(
            ImVec2(bb.Min.x, center_y - slider_height * 0.5f),
            ImVec2(value_x, center_y + slider_height * 0.5f),
            GetColorU32(theme::current.base.accent),
            slider_height * 0.5f
        );
    }

    // 滑块圆点
    const float hover_radius = thumb_radius + (thumb_radius * 0.5f * state->second.hover_anim);
    if (state->second.hover_anim > 0.0f)
    {
        draw_list->AddCircleFilled(
            ImVec2(value_x, center_y),
            hover_radius,
            GetColorU32(ImVec4(
                theme::current.base.accent.Value.x,
                theme::current.base.accent.Value.y,
                theme::current.base.accent.Value.z,
                0.2f * state->second.hover_anim
            ))
        );
    }
    
    draw_list->AddCircleFilled(
        ImVec2(value_x, center_y),
        thumb_radius,
        GetColorU32(theme::current.base.accent)
    );

    // 标签
    if (label_size.x > 0.0f)
    {
        RenderText(ImVec2(bb.Max.x + style.ItemInnerSpacing.x, bb.Min.y), label);
    }

    return pressed;
}

// 加载指示器
void LoadingIndicator(const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    const ImVec2 size = CalcItemSize(size_arg, 40.0f, 40.0f);
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect bb(pos, pos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return;

    // 动画时间
    const float time = (float)ImGui::GetTime() * 2.0f;
    
    ImDrawList* draw_list = window->DrawList;
    const ImVec2 center = bb.GetCenter();
    const float radius = size.x * 0.4f;
    const float thickness = 2.0f;
    const int num_segments = 24;

    for (int i = 0; i < num_segments; i++)
    {
        float a = i * (IM_PI * 2.0f) / (float)num_segments;
        float a_end = (i + 1) * (IM_PI * 2.0f) / (float)num_segments;
        
        // 计算每个段的透明度
        float segment_progress = (float)i / (float)num_segments;
        float alpha = ImFmod(segment_progress - time, 1.0f);
        alpha = alpha < 0.0f ? 1.0f + alpha : alpha;
        
        ImU32 color = GetColorU32(ImVec4(
            theme::current.base.accent.Value.x,
            theme::current.base.accent.Value.y,
            theme::current.base.accent.Value.z,
            alpha
        ));

        draw_list->AddBezierCubic(
            ImVec2(center.x + ImCos(a) * radius, center.y + ImSin(a) * radius),
            ImVec2(center.x + ImCos(a) * (radius + 10.0f), center.y + ImSin(a) * (radius + 10.0f)),
            ImVec2(center.x + ImCos(a_end) * (radius + 10.0f), center.y + ImSin(a_end) * (radius + 10.0f)),
            ImVec2(center.x + ImCos(a_end) * radius, center.y + ImSin(a_end) * radius),
            color,
            thickness
        );
    }
}

// 芯片控件
bool Chip(const char* label, bool* selected, bool closable)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    
    const float height = 32.0f;  // M3芯片标准高度
    const float padding = 12.0f;
    const float close_button_size = closable ? height * 0.5f : 0.0f;
    
    ImVec2 size(
        label_size.x + padding * 2.0f + close_button_size + (close_button_size > 0.0f ? padding : 0.0f),
        height
    );
    
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos, pos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // 状态管理
    static std::map<ImGuiID, internal::ElementState> states;
    auto state = states.find(id);
    if (state == states.end())
    {
        states.insert({id, internal::ElementState()});
        state = states.find(id);
    }

    // 更新动画
    state->second.hover_anim = ImLerp(
        state->second.hover_anim,
        (hovered || held) ? 1.0f : 0.0f,
        g.IO.DeltaTime * anim::SPEED
    );

    // 渲染
    ImDrawList* draw_list = window->DrawList;
    const float rounding = height * 0.5f;

    // 背景
    ImU32 bg_color;
    if (selected && *selected)
    {
        bg_color = GetColorU32(ImLerp(
            ImVec4(theme::current.base.accent),
            ImVec4(theme::current.elements.background_active),
            state->second.hover_anim
        ));
    }
    else
    {
        bg_color = GetColorU32(ImLerp(
            ImVec4(theme::current.elements.background),
            ImVec4(theme::current.elements.background_hovered),
            state->second.hover_anim
        ));
    }

    draw_list->AddRectFilled(bb.Min, bb.Max, bg_color, rounding);

    // 文本
    const ImVec2 text_pos(
        bb.Min.x + padding,
        bb.Min.y + (height - label_size.y) * 0.5f
    );
    
    if (selected && *selected)
    {
        PushStyleColor(ImGuiCol_Text, theme::current.special.checkbox_mark);
    }
    RenderText(text_pos, label);
    if (selected && *selected)
    {
        PopStyleColor();
    }

    // 关闭按钮
    bool close_clicked = false;
    if (closable)
    {
        const float close_x = bb.Max.x - padding - close_button_size;
        const float close_y = bb.Min.y + (height - close_button_size) * 0.5f;
        
        if (CloseButton(window->GetID((void*)(intptr_t)(id + 1)), 
            ImVec2(close_x, close_y)))
        {
            close_clicked = true;
        }
    }

    return close_clicked ? close_clicked : pressed;
}

} // namespace m3



namespace m3 {

// Snackbar 消息结构
struct SnackbarMessage {
    std::string text;
    std::string action;
    float show_time;
    float duration;
    std::function<void()> callback;
};

// Snackbar 管理器
class SnackbarManager {
private:
    static std::vector<SnackbarMessage> messages;
    static float current_anim;

public:
    static void Push(const char* text, float duration = 4.0f, 
                    const char* action = nullptr, 
                    std::function<void()> callback = nullptr) {
        SnackbarMessage msg;
        msg.text = text;
        msg.action = action ? action : "";
        msg.show_time = (float)ImGui::GetTime();
        msg.duration = duration;
        msg.callback = callback;
        messages.push_back(msg);
    }

    static void Render() {
        if (messages.empty()) return;

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        
        auto& msg = messages.front();
        float time_passed = (float)ImGui::GetTime() - msg.show_time;
        
        // 动画进度
        float target_anim = time_passed < msg.duration ? 1.0f : 0.0f;
        current_anim = ImLerp(current_anim, target_anim, g.IO.DeltaTime * anim::SPEED);

        if (current_anim < 0.01f && time_passed >= msg.duration) {
            messages.erase(messages.begin());
            if (!messages.empty()) {
                messages.front().show_time = (float)ImGui::GetTime();
            }
            return;
        }

        // 渲染 Snackbar
        ImDrawList* draw_list = window->DrawList;
        ImVec2 viewport_pos = ImGui::GetMainViewport()->Pos;
        ImVec2 viewport_size = ImGui::GetMainViewport()->Size;
        
        float height = 48.0f;
        float padding = 16.0f;
        ImVec2 size(viewport_size.x * 0.3f, height);
        ImVec2 pos(
            viewport_pos.x + (viewport_size.x - size.x) * 0.5f,
            viewport_pos.y + viewport_size.y - height - 24.0f
        );

        // 动画位置
        pos.y += height * (1.0f - current_anim);
        
        ImRect bb(pos, pos + size);

        // 背景
        draw_list->AddRectFilled(
            bb.Min, bb.Max,
            GetColorU32(theme::current.special.snackbar_bg),
            height * 0.25f
        );

        // 文本
        ImVec2 text_pos = bb.Min + ImVec2(padding, (height - g.FontSize) * 0.5f);
        draw_list->AddText(text_pos, GetColorU32(theme::current.special.checkbox_mark), msg.text.c_str());

        // 动作按钮
        if (!msg.action.empty() && msg.callback) {
            ImVec2 button_size = CalcTextSize(msg.action.c_str());
            ImVec2 button_pos(bb.Max.x - button_size.x - padding, bb.Min.y);
            ImRect button_bb(button_pos, button_pos + ImVec2(button_size.x + padding * 2, height));

            bool hovered = ImGui::IsMouseHoveringRect(button_bb.Min, button_bb.Max);
            bool clicked = hovered && ImGui::IsMouseClicked(0);

            if (clicked) {
                msg.callback();
            }

            ImU32 button_text_col = GetColorU32(
                hovered ? theme::current.base.accent : theme::current.special.checkbox_mark
            );
            draw_list->AddText(
                ImVec2(button_bb.Min.x + padding, button_bb.Min.y + (height - g.FontSize) * 0.5f),
                button_text_col,
                msg.action.c_str()
            );
        }
    }
};

std::vector<SnackbarMessage> SnackbarManager::messages;
float SnackbarManager::current_anim = 0.0f;

// NavigationRail 组件
bool NavigationRail(const char* items[], int* current_item, int items_count, bool show_labels = true)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    
    const float width = 80.0f;
    const float item_height = 80.0f;
    const float icon_size = 24.0f;
    
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size(width, window->WorkRect.GetHeight());
    
    ImRect bb(pos, pos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, window->GetID("##NavRail")))
        return false;

    bool value_changed = false;
    ImDrawList* draw_list = window->DrawList;

    // 背景
    draw_list->AddRectFilled(
        bb.Min, bb.Max,
        GetColorU32(theme::current.elements.background),
        0.0f
    );

    // 渲染导航项
    for (int i = 0; i < items_count; i++)
    {
        const char* item_text = items[i * 2];
        const char* item_icon = items[i * 2 + 1];
        
        ImGuiID item_id = window->GetID((void*)(intptr_t)i);
        ImVec2 item_pos(bb.Min.x, bb.Min.y + i * item_height);
        ImRect item_bb(item_pos, item_pos + ImVec2(width, item_height));

        bool hovered, held;
        bool pressed = ButtonBehavior(item_bb, item_id, &hovered, &held);
        
        if (pressed)
        {
            *current_item = i;
            value_changed = true;
        }

        // 状态管理
        static std::map<ImGuiID, internal::ElementState> states;
        auto state = states.find(item_id);
        if (state == states.end())
        {
            states.insert({item_id, internal::ElementState()});
            state = states.find(item_id);
        }

        // 更新动画
        state->second.hover_anim = ImLerp(
            state->second.hover_anim,
            (hovered || held) ? 1.0f : 0.0f,
            g.IO.DeltaTime * anim::SPEED
        );

        // 选中指示器
        if (*current_item == i)
        {
            draw_list->AddRectFilled(
                ImVec2(bb.Min.x, item_bb.Min.y + item_height * 0.1f),
                ImVec2(bb.Min.x + 3.0f, item_bb.Min.y + item_height * 0.9f),
                GetColorU32(theme::current.base.accent),
                1.5f
            );
        }

        // 悬停效果
        if (state->second.hover_anim > 0.0f)
        {
            draw_list->AddRectFilled(
                item_bb.Min, item_bb.Max,
                GetColorU32(ImVec4(
                    theme::current.elements.background_hovered.Value.x,
                    theme::current.elements.background_hovered.Value.y,
                    theme::current.elements.background_hovered.Value.z,
                    state->second.hover_anim * 0.15f
                )),
                0.0f
            );
        }

        // 图标
        ImVec2 icon_pos(
            item_bb.Min.x + (width - icon_size) * 0.5f,
            item_bb.Min.y + (show_labels ? item_height * 0.25f : (item_height - icon_size) * 0.5f)
        );
        
        PushFont(font::icomoon_page_);
        draw_list->AddText(
            icon_pos,
            GetColorU32(*current_item == i ? theme::current.base.accent : theme::current.text.primary),
            item_icon
        );
        PopFont();

        // 标签
        if (show_labels)
        {
            ImVec2 text_size = CalcTextSize(item_text);
            ImVec2 text_pos(
                item_bb.Min.x + (width - text_size.x) * 0.5f,
                item_bb.Min.y + item_height * 0.6f
            );
            
            draw_list->AddText(
                text_pos,
                GetColorU32(*current_item == i ? theme::current.base.accent : theme::current.text.primary),
                item_text
            );
        }
    }

    return value_changed;
}

// TopAppBar 组件
void TopAppBar(const char* title, bool* p_open = nullptr)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    
    const float height = 64.0f;
    const float padding = 16.0f;
    
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size(window->WorkRect.GetWidth(), height);
    
    ImRect bb(pos, pos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, window->GetID("##TopAppBar")))
        return;

    ImDrawList* draw_list = window->DrawList;

    // 背景
    draw_list->AddRectFilled(
        bb.Min, bb.Max,
        GetColorU32(theme::current.elements.background),
        0.0f
    );

    // 分割线
    draw_list->AddLine(
        ImVec2(bb.Min.x, bb.Max.y),
        bb.Max,
        GetColorU32(theme::current.separator),
        1.0f
    );

    float current_x = bb.Min.x + padding;

    // 导航图标按钮
    const float icon_size = 24.0f;
    ImRect nav_bb(
        ImVec2(current_x, bb.Min.y + (height - icon_size) * 0.5f),
        ImVec2(current_x + icon_size, bb.Min.y + (height + icon_size) * 0.5f)
    );

    bool nav_hovered = ImGui::IsMouseHoveringRect(nav_bb.Min, nav_bb.Max);
    bool nav_clicked = nav_hovered && ImGui::IsMouseClicked(0);

    if (nav_hovered)
    {
        draw_list->AddCircleFilled(
            nav_bb.GetCenter(),
            icon_size * 0.7f,
            GetColorU32(ImVec4(
                theme::current.elements.background_hovered.Value.x,
                theme::current.elements.background_hovered.Value.y,
                theme::current.elements.background_hovered.Value.z,
                0.15f
            ))
        );
    }

    PushFont(font::icomoon_page_);
    draw_list->AddText(
        nav_bb.Min,
        GetColorU32(theme::current.text.primary),
        icons::MENU
    );
    PopFont();

    current_x += icon_size + padding;

    // 标题
    ImVec2 title_size = CalcTextSize(title);
    draw_list->AddText(
        ImVec2(current_x, bb.Min.y + (height - title_size.y) * 0.5f),
        GetColorU32(theme::current.text.primary),
        title
    );

    // 右侧操作按钮
    if (p_open)
    {
        ImRect close_bb(
            ImVec2(bb.Max.x - icon_size - padding, bb.Min.y + (height - icon_size) * 0.5f),
            ImVec2(bb.Max.x - padding, bb.Min.y + (height + icon_size) * 0.5f)
        );

        bool close_hovered = ImGui::IsMouseHoveringRect(close_bb.Min, close_bb.Max);
        bool close_clicked = close_hovered && ImGui::IsMouseClicked(0);

        if (close_hovered)
        {
            draw_list->AddCircleFilled(
                close_bb.GetCenter(),
                icon_size * 0.7f,
                GetColorU32(ImVec4(
                    theme::current.elements.background_hovered.Value.x,
                    theme::current.elements.background_hovered.Value.y,
                    theme::current.elements.background_hovered.Value.z,
                    0.15f
                ))
            );
        }

        PushFont(font::icomoon_page_);
        draw_list->AddText(
            close_bb.Min,
            GetColorU32(theme::current.text.primary),
            icons::CLOSE
        );
        PopFont();

        if (close_clicked)
            *p_open = false;
    }
}

} // namespace m3

namespace m3 {

// 进度条控件
void ProgressBar(float fraction, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
    ImRect bb(pos, pos + size);
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return;

    // 渲染
    ImDrawList* draw_list = window->DrawList;
    const float rounding = size.y * 0.5f;

    // 背景
    draw_list->AddRectFilled(
        bb.Min,
        bb.Max,
        GetColorU32(theme::current.elements.background),
        rounding
    );

    // 进度
    if (fraction > 0.0f)
    {
        float progress_width = ImLerp(0.0f, size.x, fraction);
        draw_list->AddRectFilled(
            bb.Min,
            ImVec2(bb.Min.x + progress_width, bb.Max.y),
            GetColorU32(theme::current.base.accent),
            rounding,
            progress_width < rounding ? ImDrawFlags_RoundCornersLeft : ImDrawFlags_RoundCornersAll
        );
    }
}

// 加载指示器
void LoadingSpinner(const char* label, float radius, float thickness, const ImU32& color) 
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

    const ImRect bb(pos, pos + size);
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return;

    // 动画
    const float t = (float)g.Time;
    const auto degree_offset = 0.f;

    window->DrawList->PathClear();
    const size_t num_segments = 30;
    const float start = (t * 8) * IM_PI * 2.0f / num_segments;
    
    const float aoffset = start;
    const float bofsset = aoffset + (IM_PI * 2.0f) * 0.85f;
    const float xpos = pos.x + radius;
    const float ypos = pos.y + radius;

    for (size_t i = 0; i <= num_segments; i++) 
    {
        const float a = aoffset + (i * (bofsset - aoffset) / num_segments);
        window->DrawList->PathLineTo(ImVec2(
            xpos + ImCos(a) * radius,
            ypos + ImSin(a) * radius
        ));
    }
    window->DrawList->PathStroke(color, false, thickness);
}

// 标签页控件
bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    static std::map<ImGuiID, internal::ElementState> states;
    ImGuiID id = window->GetID(str_id);
    auto state = states.find(id);
    if (state == states.end())
    {
        states.insert({id, internal::ElementState()});
        state = states.find(id);
    }

    // 更新状态动画
    state->second.UpdateColors(g.IO.DeltaTime);

    // 渲染标签栏背景
    ImRect tab_bar_bb = window->DC.CursorRect;
    tab_bar_bb.Max.x = window->WorkRect.Max.x;
    tab_bar_bb.Max.y += g.FontSize + g.Style.FramePadding.y * 2;

    return ImGui::BeginTabBar(str_id, flags);
}

bool TabItem(const char* label, bool* p_open, ImGuiTabItemFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    static std::map<ImGuiID, internal::ElementState> states;
    ImGuiID id = window->GetID(label);
    auto state = states.find(id);
    if (state == states.end())
    {
        states.insert({id, internal::ElementState()});
        state = states.find(id);
    }

    // 更新状态动画
    state->second.UpdateColors(g.IO.DeltaTime);

    return ImGui::TabItem(label, p_open, flags);
}

// 工具提示
void Tooltip(const char* text)
{
    ImGuiContext& g = *GImGui;
    if (g.HoveredId != 0)
    {
        BeginTooltip();
        TextUnformatted(text);
        EndTooltip();
    }
}

// 可折叠标题
bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    static std::map<ImGuiID, internal::ElementState> states;
    ImGuiID id = window->GetID(label);
    auto state = states.find(id);
    if (state == states.end())
    {
        states.insert({id, internal::ElementState()});
        state = states.find(id);
    }

    // 更新状态动画
    state->second.UpdateColors(g.IO.DeltaTime);

    return TreeNodeEx(label, flags | ImGuiTreeNodeFlags_CollapsingHeader);
}

} // namespace m3

namespace m3 {

// Snackbar 消息结构
struct SnackbarMessage {
    std::string text;
    std::string action;
    float show_time;
    float duration;
    std::function<void()> callback;
};

// Snackbar 管理器
class SnackbarManager {
private:
    static std::vector<SnackbarMessage> messages;
    static float current_anim;

public:
    static void Push(const char* text, float duration = 4.0f, 
                    const char* action = nullptr, 
                    std::function<void()> callback = nullptr) {
        SnackbarMessage msg;
        msg.text = text;
        msg.action = action ? action : "";
        msg.show_time = (float)ImGui::GetTime();
        msg.duration = duration;
        msg.callback = callback;
        messages.push_back(msg);
    }

    static void Render() {
        if (messages.empty()) return;

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        
        auto& msg = messages.front();
        float time_passed = (float)ImGui::GetTime() - msg.show_time;
        
        // 动画进度
        float target_anim = time_passed < msg.duration ? 1.0f : 0.0f;
        current_anim = ImLerp(current_anim, target_anim, g.IO.DeltaTime * anim::SPEED);

        if (current_anim < 0.01f && time_passed >= msg.duration) {
            messages.erase(messages.begin());
            if (!messages.empty()) {
                messages.front().show_time = (float)ImGui::GetTime();
            }
            return;
        }

        // 渲染 Snackbar
        ImDrawList* draw_list = window->DrawList;
        ImVec2 viewport_pos = ImGui::GetMainViewport()->Pos;
        ImVec2 viewport_size = ImGui::GetMainViewport()->Size;
        
        float height = 48.0f;
        float padding = 16.0f;
        ImVec2 size(viewport_size.x * 0.3f, height);
        ImVec2 pos(
            viewport_pos.x + (viewport_size.x - size.x) * 0.5f,
            viewport_pos.y + viewport_size.y - height - 24.0f
        );

        // 动画位置
        pos.y += height * (1.0f - current_anim);
        
        ImRect bb(pos, pos + size);

        // 背景
        draw_list->AddRectFilled(
            bb.Min, bb.Max,
            GetColorU32(theme::current.special.snackbar_bg),
            height * 0.25f
        );

        // 文本
        ImVec2 text_pos = bb.Min + ImVec2(padding, (height - g.FontSize) * 0.5f);
        draw_list->AddText(text_pos, GetColorU32(theme::current.special.checkbox_mark), msg.text.c_str());

        // 动作按钮
        if (!msg.action.empty() && msg.callback) {
            ImVec2 button_size = CalcTextSize(msg.action.c_str());
            ImVec2 button_pos(bb.Max.x - button_size.x - padding, bb.Min.y);
            ImRect button_bb(button_pos, button_pos + ImVec2(button_size.x + padding * 2, height));

            bool hovered = ImGui::IsMouseHoveringRect(button_bb.Min, button_bb.Max);
            bool clicked = hovered && ImGui::IsMouseClicked(0);

            if (clicked) {
                msg.callback();
            }

            ImU32 button_text_col = GetColorU32(
                hovered ? theme::current.base.accent : theme::current.special.checkbox_mark
            );
            draw_list->AddText(
                ImVec2(button_bb.Min.x + padding, button_bb.Min.y + (height - g.FontSize) * 0.5f),
                button_text_col,
                msg.action.c_str()
            );
        }
    }
};

std::vector<SnackbarMessage> SnackbarManager::messages;
float SnackbarManager::current_anim = 0.0f;

// NavigationRail 组件
bool NavigationRail(const char* items[], int* current_item, int items_count, bool show_labels = true)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    
    const float width = 80.0f;
    const float item_height = 80.0f;
    const float icon_size = 24.0f;
    
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size(width, window->WorkRect.GetHeight());
    
    ImRect bb(pos, pos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, window->GetID("##NavRail")))
        return false;

    bool value_changed = false;
    ImDrawList* draw_list = window->DrawList;

    // 背景
    draw_list->AddRectFilled(
        bb.Min, bb.Max,
        GetColorU32(theme::current.elements.background),
        0.0f
    );

    // 渲染导航项
    for (int i = 0; i < items_count; i++)
    {
        const char* item_text = items[i * 2];
        const char* item_icon = items[i * 2 + 1];
        
        ImGuiID item_id = window->GetID((void*)(intptr_t)i);
        ImVec2 item_pos(bb.Min.x, bb.Min.y + i * item_height);
        ImRect item_bb(item_pos, item_pos + ImVec2(width, item_height));

        bool hovered, held;
        bool pressed = ButtonBehavior(item_bb, item_id, &hovered, &held);
        
        if (pressed)
        {
            *current_item = i;
            value_changed = true;
        }

        // 状态管理
        static std::map<ImGuiID, internal::ElementState> states;
        auto state = states.find(item_id);
        if (state == states.end())
        {
            states.insert({item_id, internal::ElementState()});
            state = states.find(item_id);
        }

        // 更新动画
        state->second.hover_anim = ImLerp(
            state->second.hover_anim,
            (hovered || held) ? 1.0f : 0.0f,
            g.IO.DeltaTime * anim::SPEED
        );

        // 选中指示器
        if (*current_item == i)
        {
            draw_list->AddRectFilled(
                ImVec2(bb.Min.x, item_bb.Min.y + item_height * 0.1f),
                ImVec2(bb.Min.x + 3.0f, item_bb.Min.y + item_height * 0.9f),
                GetColorU32(theme::current.base.accent),
                1.5f
            );
        }

        // 悬停效果
        if (state->second.hover_anim > 0.0f)
        {
            draw_list->AddRectFilled(
                item_bb.Min, item_bb.Max,
                GetColorU32(ImVec4(
                    theme::current.elements.background_hovered.Value.x,
                    theme::current.elements.background_hovered.Value.y,
                    theme::current.elements.background_hovered.Value.z,
                    state->second.hover_anim * 0.15f
                )),
                0.0f
            );
        }

        // 图标
        ImVec2 icon_pos(
            item_bb.Min.x + (width - icon_size) * 0.5f,
            item_bb.Min.y + (show_labels ? item_height * 0.25f : (item_height - icon_size) * 0.5f)
        );
        
        PushFont(font::icomoon_page_);
        draw_list->AddText(
            icon_pos,
            GetColorU32(*current_item == i ? theme::current.base.accent : theme::current.text.primary),
            item_icon
        );
        PopFont();

        // 标签
        if (show_labels)
        {
            ImVec2 text_size = CalcTextSize(item_text);
            ImVec2 text_pos(
                item_bb.Min.x + (width - text_size.x) * 0.5f,
                item_bb.Min.y + item_height * 0.6f
            );
            
            draw_list->AddText(
                text_pos,
                GetColorU32(*current_item == i ? theme::current.base.accent : theme::current.text.primary),
                item_text
            );
        }
    }

    return value_changed;
}

// TopAppBar 组件
void TopAppBar(const char* title, bool* p_open = nullptr)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    
    const float height = 64.0f;
    const float padding = 16.0f;
    
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size(window->WorkRect.GetWidth(), height);
    
    ImRect bb(pos, pos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, window->GetID("##TopAppBar")))
        return;

    ImDrawList* draw_list = window->DrawList;

    // 背景
    draw_list->AddRectFilled(
        bb.Min, bb.Max,
        GetColorU32(theme::current.elements.background),
        0.0f
    );

    // 分割线
    draw_list->AddLine(
        ImVec2(bb.Min.x, bb.Max.y),
        bb.Max,
        GetColorU32(theme::current.separator),
        1.0f
    );

    float current_x = bb.Min.x + padding;

    // 导航图标按钮
    const float icon_size = 24.0f;
    ImRect nav_bb(
        ImVec2(current_x, bb.Min.y + (height - icon_size) * 0.5f),
        ImVec2(current_x + icon_size, bb.Min.y + (height + icon_size) * 0.5f)
    );

    bool nav_hovered = ImGui::IsMouseHoveringRect(nav_bb.Min, nav_bb.Max);
    bool nav_clicked = nav_hovered && ImGui::IsMouseClicked(0);

    if (nav_hovered)
    {
        draw_list->AddCircleFilled(
            nav_bb.GetCenter(),
            icon_size * 0.7f,
            GetColorU32(ImVec4(
                theme::current.elements.background_hovered.Value.x,
                theme::current.elements.background_hovered.Value.y,
                theme::current.elements.background_hovered.Value.z,
                0.15f
            ))
        );
    }

    PushFont(font::icomoon_page_);
    draw_list->AddText(
        nav_bb.Min,
        GetColorU32(theme::current.text.primary),
        icons::MENU
    );
    PopFont();

    current_x += icon_size + padding;

    // 标题
    ImVec2 title_size = CalcTextSize(title);
    draw_list->AddText(
        ImVec2(current_x, bb.Min.y + (height - title_size.y) * 0.5f),
        GetColorU32(theme::current.text.primary),
        title
    );

    // 右侧操作按钮
    if (p_open)
    {
        ImRect close_bb(
            ImVec2(bb.Max.x - icon_size - padding, bb.Min.y + (height - icon_size) * 0.5f),
            ImVec2(bb.Max.x - padding, bb.Min.y + (height + icon_size) * 0.5f)
        );

        bool close_hovered = ImGui::IsMouseHoveringRect(close_bb.Min, close_bb.Max);
        bool close_clicked = close_hovered && ImGui::IsMouseClicked(0);

        if (close_hovered)
        {
            draw_list->AddCircleFilled(
                close_bb.GetCenter(),
                icon_size * 0.7f,
                GetColorU32(ImVec4(
                    theme::current.elements.background_hovered.Value.x,
                    theme::current.elements.background_hovered.Value.y,
                    theme::current.elements.background_hovered.Value.z,
                    0.15f
                ))
            );
        }

        PushFont(font::icomoon_page_);
        draw_list->AddText(
            close_bb.Min,
            GetColorU32(theme::current.text.primary),
            icons::CLOSE
        );
        PopFont();

        if (close_clicked)
            *p_open = false;
    }
}

} // namespace m3

