#include "custom_widgets.hpp"
#include <map>

using namespace std;
namespace font
{
    ImFont *icomoon_logo_;
    ImFont *icomoon_page_;
    ImFont *inter_semibold_;
    ImFont *icon_notify_;
}

namespace ImGui
{
   ImU32  GetColorU32(const ImVec4& col, float alpha_mul){
        ImGuiStyle& style = GImGui->Style;
        ImVec4 c = col;
        c.w *= style.Alpha * alpha_mul;
        return ColorConvertFloat4ToU32(c);
    };
   ImU32  GetColorU32(const ImColor& col){
        ImGuiStyle& style = GImGui->Style;
        ImVec4 c = col.Value;
        c.w *= style.Alpha;
        return ColorConvertFloat4ToU32(c);
    };
    ImU32  GetColorU32(const ImColor& col, float alpha_mul){
        ImGuiStyle& style = GImGui->Style;
        ImVec4 c = col.Value;
        c.w *= style.Alpha * alpha_mul;
        return ColorConvertFloat4ToU32(c);
    };

}
namespace custom
{

    // // 初始化主题配置
    // namespace theme
    // {
    //     Colors colors;
    //     Metrics metrics;
    // }


    bool ChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool cap, ImGuiWindowFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* parent_window = g.CurrentWindow;

        flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
        flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);

        const ImVec2 content_avail = GetContentRegionAvail();
        ImVec2 size = ImTrunc(size_arg);
        const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
        if (size.x <= 0.0f) size.x = ImMax(content_avail.x + size.x, 4.0f);
        if (size.y <= 0.0f) size.y = ImMax(content_avail.y + size.y, 4.0f);

        const ImVec2 CursorPos = parent_window->DC.CursorPos;
        ImVec2 Pos = CursorPos + ImVec2(0, cap ? 45 : 0);
        ImVec2 Size = size - ImVec2(0, cap ? 45 : 0);

        SetNextWindowPos(Pos);
        SetNextWindowSize(Size);

        GetWindowDrawList()->AddRectFilled(Pos, CursorPos + size_arg, 
            GetColorU32(theme::current.child.background), internal::rounding, 
            cap ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_RoundCornersAll);

        if (cap) {

            GetWindowDrawList()->AddRectFilledMultiColor(
                CursorPos + ImVec2((size_arg.x / 2), 44),
                CursorPos + ImVec2((size_arg.x - 50), 45),
                GetColorU32(theme::current.base.accent), 
                GetColorU32(theme::current.base.accent, 0.0f),
                GetColorU32(theme::current.base.accent, 0.0f), 
                GetColorU32(theme::current.base.accent));

            GetWindowDrawList()->AddRectFilledMultiColor(
            CursorPos + ImVec2(50, 44),
             CursorPos + ImVec2(size_arg.x / 2, 45), 
             GetColorU32(theme::current.base.accent, 0.0f), 
             GetColorU32(theme::current.base.accent), 
             GetColorU32(theme::current.base.accent), 
             GetColorU32(theme::current.base.accent, 0.0f));

            GetWindowDrawList()->AddRectFilled(
                CursorPos, CursorPos + ImVec2(size_arg.x, 35),
             GetColorU32(theme::current.base.accent), internal::rounding, ImDrawFlags_RoundCornersTop);
            ImVec2 text_size = CalcTextSize(name);
            GetWindowDrawList()->AddText(
                CursorPos + ImVec2(size_arg.x - text_size.x, 35 - text_size.y) * 0.5f,
                 GetColorU32(theme::current.text.active), name);
        }

        const char* temp_window_name;

        if (name) ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);

        else ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

        const float backup_border_size = g.Style.ChildBorderSize;

        bool ret = Begin(temp_window_name, NULL, flags | ImGuiWindowFlags_NoBackground);

        ImGuiWindow* child_window = g.CurrentWindow;
        child_window->ChildId = id;
        //child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

        if (child_window->BeginCount == 1) parent_window->DC.CursorPos = child_window->Pos;

        const ImGuiID temp_id_for_activation = ImHashStr("##Child", 0, id);
        if (g.ActiveId == temp_id_for_activation) ClearActiveID();

        if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) 
        && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavWindowHasScrollY))
        {
            FocusWindow(child_window);
            NavInitWindow(child_window, false);
            SetActiveID(temp_id_for_activation, child_window);
            g.ActiveIdSource = g.NavInputSource;
        }
        return ret;
    }

    bool Child(const char* str_id, const ImVec2& size_arg, bool cap, ImGuiWindowFlags extra_flags)
    {
        ImGuiWindow* window = GetCurrentWindow();

        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(13, 13));
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(13, 13));

        return ChildEx(str_id, window->GetID(str_id), size_arg, cap, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding);
    }

    bool ChildID(ImGuiID id, const ImVec2& size_arg, bool cap, ImGuiWindowFlags extra_flags)
    {
        IM_ASSERT(id != 0);
        return ChildEx(NULL, id, size_arg, cap, extra_flags);
    }

    void EndChild()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        PopStyleVar(2);

  
        IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);

        g.WithinEndChildID = window->ID;
        if (window->BeginCount > 1)
        {
            End();
        }
        else
        {
            ImVec2 sz = window->Size;
            End();

            ImGuiWindow* parent_window = g.CurrentWindow;
            ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
            ItemSize(sz);
            if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavWindowHasScrollY) 
                    && !(window->Flags & ImGuiWindowFlags_NavFlattened))
            {
                ItemAdd(bb, window->ChildId);
            }
            else
            {
                ItemAdd(bb, 0);
                if (window->Flags & ImGuiWindowFlags_NavFlattened) 
                parent_window->DC.NavLayersActiveMaskNext |= window->DC.NavLayersActiveMaskNext;
            }
            if (g.HoveredWindow == window) 
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
        }
        g.WithinEndChildID = 0;
        g.LogLinePosY = -FLT_MAX;
    }


    void BeginGroup()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;

        g.GroupStack.resize(g.GroupStack.Size + 1);
        ImGuiGroupData& group_data = g.GroupStack.back();
        group_data.WindowID = window->ID;
        group_data.BackupCursorPos = window->DC.CursorPos;
        group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
        group_data.BackupIndent = window->DC.Indent;
        group_data.BackupGroupOffset = window->DC.GroupOffset;
        group_data.BackupCurrLineSize = window->DC.CurrLineSize;
        group_data.BackupCurrLineTextBaseOffset = window->DC.CurrLineTextBaseOffset;
        group_data.BackupActiveIdIsAlive = g.ActiveIdIsAlive;
        group_data.BackupHoveredIdIsAlive = g.HoveredId != 0;
        group_data.BackupIsSameLine = window->DC.IsSameLine;
        group_data.BackupDeactivatedIdIsAlive = g.DeactivatedItemData.IsAlive;
        group_data.EmitItem = true;

        window->DC.GroupOffset.x = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffset.x;
        window->DC.Indent = window->DC.GroupOffset;
        window->DC.CursorMaxPos = window->DC.CursorPos;
        window->DC.CurrLineSize = ImVec2(0.0f, 0.0f);
        if (g.LogEnabled) g.LogLinePosY = -FLT_MAX;
    }

    void EndGroup()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        IM_ASSERT(g.GroupStack.Size > 0);

        ImGuiGroupData& group_data = g.GroupStack.back();
        IM_ASSERT(group_data.WindowID == window->ID);

        if (window->DC.IsSetPos) ErrorCheckUsingSetCursorPosToExtendParentBoundaries();

        ImRect group_bb(group_data.BackupCursorPos, ImMax(window->DC.CursorMaxPos, group_data.BackupCursorPos));

        window->DC.CursorPos = group_data.BackupCursorPos;
        window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
        window->DC.Indent = group_data.BackupIndent;
        window->DC.GroupOffset = group_data.BackupGroupOffset;
        window->DC.CurrLineSize = group_data.BackupCurrLineSize;
        window->DC.CurrLineTextBaseOffset = group_data.BackupCurrLineTextBaseOffset;
        if (g.LogEnabled) g.LogLinePosY = -FLT_MAX;

        if (!group_data.EmitItem)
        {
            g.GroupStack.pop_back();
            return;
        }

        window->DC.CurrLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrLineTextBaseOffset);
        ItemSize(group_bb.GetSize());
        ItemAdd(group_bb, 0, NULL, ImGuiItemFlags_NoTabStop);
    // 抄imgui 的endgroup实现
    const bool group_contains_curr_active_id = (group_data.BackupActiveIdIsAlive != g.ActiveId) && (g.ActiveIdIsAlive == g.ActiveId) && g.ActiveId;
    const bool group_contains_deactivated_id = (group_data.BackupDeactivatedIdIsAlive == false) && (g.DeactivatedItemData.IsAlive == true);
    if (group_contains_curr_active_id)
        g.LastItemData.ID = g.ActiveId;
    else if (group_contains_deactivated_id)
        g.LastItemData.ID = g.DeactivatedItemData.ID;
    g.LastItemData.Rect = group_bb;

        const bool group_contains_curr_hovered_id = (group_data.BackupHoveredIdIsAlive == false) && g.HoveredId != 0;
        if (group_contains_curr_hovered_id) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;

        if (group_contains_curr_active_id && g.ActiveIdHasBeenEditedThisFrame) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Edited;

    // Forward Deactivated flag
    g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDeactivated;
    if (group_contains_deactivated_id)
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Deactivated;

        g.GroupStack.pop_back();
    }



    bool Page(bool selected, const char *icon, const ImVec2 &size_arg)
    {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;
        // 获取图标ID （hash）
        const ImGuiID id = window->GetID(icon);
        // 获取图标大小
        const ImVec2 label_size = CalcTextSize(icon, NULL, true);
        // 获取图标位置
        const ImVec2 pos = window->DC.CursorPos;
        // 获取图标大小
        ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);
        // 计算按钮区域
        const ImRect bb(pos, pos + size);
        // 设置Item大小
        ItemSize(size, 0.f);
        // 添加Item
        if (!ItemAdd(bb, id))
            return false;

        // 处理交互
        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);

        // 状态管理
        static map<ImGuiID, internal::PageState> states;
        // auto& state = states[id];
        auto state = states.find(id);
        if (state == states.end())
        {
            states.insert({id, internal::PageState()});
            state = states.find(id);
        }

        // 动画更新
        ImGuiContext &g = *GImGui;

            // 更新状态
        state->second.state.active = selected;
        state->second.state.hovered = hovered;
        auto delta_time = g.IO.DeltaTime;
        // 更新颜色和动画
        state->second.UpdateColors(delta_time);
        state->second.page_anim.line_alpha = 
                ImLerp(state->second.page_anim.line_alpha,
                 selected ? 1.f : 0.f, delta_time * anim::SPEED);
        state->second.page_anim.line_size = 
                ImLerp(state->second.page_anim.line_size,
                 selected ? size.x / 4 : size.x / 2, 
                 delta_time * anim::SPEED);

        state->second.page_anim.smooth_swap = ImLerp(
            state->second.page_anim.smooth_swap,
            selected ? 26.f : 0,
            delta_time * anim::SPEED * 2.0f);

        // 绘制背景
        window->DrawList->AddRectFilled(
            bb.Min,
            bb.Max,
            GetColorU32(state->second.colors.background),
            internal::rounding);
        window->DrawList->AddRectFilled(
            bb.Min + ImVec2(+state->second.page_anim.line_size, (size_arg.y + 9)),
             bb.Max + ImVec2(-state->second.page_anim.line_size, 15),
              GetColorU32(state->second.colors.accent, state->second.page_anim.line_alpha),
               internal::rounding, ImDrawFlags_RoundCornersTop);

        // 裁剪绘制区域
        PushClipRect(bb.Min, bb.Max, true);
        // 设置字体
        PushFont(font::icomoon_page_);

        // 计算并绘制图标文本
        const ImVec2 text_size = CalcTextSize(icon);
        const ImVec2 text_pos(
            bb.Min.x + (size_arg.x - text_size.x) * 0.5f,          // 水平居中
            bb.Max.y - text_size.y - (size.y - text_size.y) * 0.5f //
        );

        window->DrawList->AddText(
            ImVec2(text_pos.x, text_pos.y + state->second.page_anim.smooth_swap),
            GetColorU32(state->second.colors.text),
            icon);
        window->DrawList->AddText(
            ImVec2(text_pos.x, text_pos.y - 25 + state->second.page_anim.smooth_swap),
            GetColorU32(state->second.colors.text),
            icon);
        // 恢复字体
        PopFont();
        // 恢复裁剪区域
        PopClipRect();

        return pressed;
    }

    bool Button(const char *label, const ImVec2 &size_arg)
    {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;
        // 获取按钮ID(hash)
        const ImGuiID id = window->GetID(label);
        // 获取按钮位置
        const ImVec2 pos = window->DC.CursorPos;
        // 获取lable大小
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

        // 计算按钮大小
        const ImRect bb(pos, pos + size);
        // 设置Item大小
        ItemSize(size, 0.f);
        // 添加Item
        if (!ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);

        // 状态管理
        static map<ImGuiID, internal::ButtonState> states;
        auto state = states.find(id);
        if (state == states.end())
        {
            states.insert({id, internal::ButtonState()});
            state = states.find(id);
        }

        // 动画更新
        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        // 背景颜色过渡
        state->second.colors.background = ImLerp(
            state->second.colors.background,
            IsItemActive() || hovered ? theme::current.elements.background_active : theme::current.elements.background,
            g.IO.DeltaTime * anim::SPEED);

        // 文本颜色过渡
        state->second.colors.text = ImLerp(
            state->second.colors.text,
            IsItemActive() || hovered ? theme::current.text.active : theme::current.text.disabled,
            g.IO.DeltaTime * anim::SPEED);

        // 绘制背景
        window->DrawList->AddRectFilled(
            bb.Min,
            bb.Max,
            GetColorU32(state->second.colors.background),
            internal::rounding);

        // 裁剪绘制区域
        PushClipRect(bb.Min, bb.Max, true);
        // 设置字体
        PushFont(font::icomoon_page_);

        // 计算并绘制文本
        const ImVec2 text_size = CalcTextSize(label);
        const ImVec2 text_pos(
            bb.Min.x + (size_arg.x - text_size.x) * 0.5f,
            bb.Max.y - text_size.y - (size.y - text_size.y) * 0.5f);
        //渲染按钮文本
        window->DrawList->AddText(
            text_pos,
            GetColorU32(state->second.text),
            label);
        // 恢复字体
        PopFont();
        // 恢复裁剪区域
        PopClipRect();

        return pressed;
    }

    bool Checkbox(const char *label, bool *v)
    {
        // 获取当前窗口
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);
        // 获取当前光标位置
        const ImVec2 pos = window->DC.CursorPos;
        // 获取标签大小
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        // 计算复选框大小和位置
        const float square_sz = 20; // GetFrameHeight();  // 使用标准框架高度
        // 获取当前窗口内容区域最大值
        const float w = GetContentRegionMax().x - style.WindowPadding.x;
        // 计算复选框总大小
        const ImRect total_bb(
             pos + ImVec2(w - square_sz, 0),
            pos + ImVec2(w, 32));
        // 设置Item大小
        ItemSize(total_bb, 0.f);
        // 添加Item
        if (!ItemAdd(total_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
        // if (pressed) {
        //     *v = !(*v);  // 切换状态
        //     MarkItemEdited(id);
        // }

        // 状态管理
        static std::map<ImGuiID, internal::CheckboxState> states;
        auto state = states.find(id);
        if (state == states.end())
        {
            states.insert({id, internal::CheckboxState()});
            state = states.find(id);
        }

        // 动画更新
        state->second.background = ImLerp(
            state->second.background,
            *v ? theme::colors.accent : (hovered ? theme::colors.background_light : theme::colors.background),
            g.IO.DeltaTime * anim::SPEED);
        // 勾选标记动画
        state->second.alpha_mark = ImLerp(
            state->second.alpha_mark,
            *v ? 1.0f : 0.0f,
            g.IO.DeltaTime * anim::SPEED);
        // 文本颜色动画
        state->second.text = ImLerp(
            state->second.text,
            *v ? theme::colors.accent : (hovered ? theme::colors.text_hov : theme::colors.text),
            g.IO.DeltaTime * anim::SPEED);
        // // 缩放动画
        // state->second.scale = ImLerp(
        //     state->second.scale,
        //     pressed ? 0.85f : (hovered ? 1.05f : 1.0f),  // 按下缩小,悬停放大
        //     g.IO.DeltaTime * anim::SPEED
        // );
        if (IsItemClicked())
        {
            *v = !(*v);
            MarkItemEdited(id);
        }
        // 绘制复选框
        const ImRect check_bb(
            pos + ImVec2(w - square_sz, (32 - square_sz) / 2),
            pos + ImVec2(w, (32 + square_sz) / 2));

        // 绘制背景
        window->DrawList->AddRectFilled(
            check_bb.Min,
            check_bb.Max,
            GetColorU32(state->second.background),
            theme::metrics.rounding);
        // 绘制勾选标记
        const ImVec2 center = check_bb.Min + ImVec2(square_sz / 2, square_sz / 2) * 0.5f;
        RenderCheckMark(window->DrawList,
                        center,
                        GetColorU32(theme::colors.text),
                        square_sz / 2);

        // 绘制文本
        const ImVec2 text_pos = pos + ImVec2(0, (32 - CalcTextSize(label).y) / 2);

        window->DrawList->AddText(text_pos, GetColorU32(state->second.text), label);

        return pressed;
    }

    bool CheckboxClicked(const char* label, bool* v)
    {
        ImGuiWindow* window = GetCurrentWindow();

        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true), pos = window->DC.CursorPos;
        const ImRect bb(pos + ImVec2(GetContentRegionMax().x - 70, 0), 
        pos + ImVec2(GetContentRegionMax().x - 50, 32));

        if (!ItemAdd(bb, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);

        custom::Checkbox(label, v);

        PushFont(font::icomoon_page_);
        GetWindowDrawList()->AddText(pos + ImVec2(GetContentRegionMax().x - 65, 7), 
        GetColorU32(theme::colors.text), "l");
        PopFont();

        return pressed;
    }


    int rotation_start_index;
    void ImRotateStart()
    {
        rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
    }
    ImVec2 ImRotationCenter()
    {
        ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

        const auto &buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

        return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
    }
    void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
    {
        float s = sin(rad), c = cos(rad);
        center = ImRotate(center, s, c) - center;

        auto &buf = ImGui::GetWindowDrawList()->VtxBuffer;
        for (int i = rotation_start_index; i < buf.Size; i++)
            buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
    }
    
    bool BeginCombo(const char* label, const char* preview_value, int val, bool multi, ImGuiComboFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();

        g.NextWindowData.ClearFlags();
        if (window->SkipItems) return false;

        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        const float w = ((GetContentRegionMax().x - style.WindowPadding.x));

        const ImRect bb(window->DC.CursorPos + ImVec2(w / 2, 0), window->DC.CursorPos + ImVec2(w, 32));
        const ImRect total_bb(bb.Min, bb.Max);
        ItemSize(bb, 0.f);

        if (!ItemAdd(total_bb, id, &bb)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held);

        static std::map<ImGuiID, internal::ComboState> states;
        auto state = states.find(id);

        if (state == states.end())
        {
            states.insert({ id, internal::ComboState() });
            state = states.find(id);
        }

        if (hovered && g.IO.MouseClicked[0] || 
        state->second.opened && g.IO.MouseClicked[0] 
        && !state->second.hovered) state->second.opened = !state->second.opened;

        state->second.arrow_roll = ImLerp(state->second.arrow_roll,
         state->second.opened ? -1.f : 1.f, g.IO.DeltaTime * anim::SPEED);
        state->second.text = ImLerp(state->second.text, 
            state->second.opened ? theme::colors.text_active : 
               hovered ? theme::colors.text_hov : theme::colors.text,
                g.IO.DeltaTime * anim::SPEED);
        state->second.background = ImLerp(state->second.background, 
           state->second.opened ? theme::colors.background_active : theme::colors.background,
            g.IO.DeltaTime * anim::SPEED);
        state->second.combo_size = ImLerp(state->second.combo_size,
            state->second.opened ? (val * 30) + 15 : 0.f, 
            g.IO.DeltaTime * anim::SPEED);

        GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max,
             GetColorU32(state->second.background), theme::metrics.rounding);

        GetWindowDrawList()->AddText(
            ImVec2(window->DC.CursorPos.x, bb.Min.y + (32 - CalcTextSize(label).y) / 2 + 1),
             GetColorU32(state->second.text), label);

        PushClipRect(bb.Min, bb.Max, true);
        GetWindowDrawList()->AddText(ImVec2(bb.Min.x + ((32 - CalcTextSize(label).y) / 2),
            bb.Min.y + (32 - CalcTextSize(label).y) / 2), 
            GetColorU32(theme::colors.text_active), preview_value);
        PopClipRect();

        GetWindowDrawList()->AddRectFilled(bb.Max - ImVec2(32, 32), 
                bb.Max, GetColorU32(state->second.background), theme::metrics.rounding);
        GetWindowDrawList()->AddRectFilledMultiColor(
            bb.Max - ImVec2(120, 32), bb.Max,
             GetColorU32(state->second.background, 0.f),
              GetColorU32(state->second.background, 1.f), 
              GetColorU32(state->second.background, 1.f),
                GetColorU32(state->second.background, 0.f));

        PushFont(font::icomoon_page_);
        ImRotateStart();
        const ImVec2 text_size = CalcTextSize("i");
        GetWindowDrawList()->AddText(
            ImVec2(bb.Max.x - (32 + text_size.y) / 2,
             bb.Min.y + (32 - text_size.y) / 2),
              GetColorU32(theme::colors.text), "i");
        ImRotateEnd(1.57f * state->second.arrow_roll);
        PopFont();

        if (!IsRectVisible(bb.Min, bb.Max + ImVec2(0, 2)))
        {
            state->second.opened = false;
            state->second.combo_size = 0.f;
        }

        if (!state->second.opened && state->second.combo_size < 2.f) return false;

        ImGui::SetNextWindowPos(ImVec2(bb.Min.x, bb.Max.y + 5));
        ImGui::SetNextWindowSize(ImVec2(bb.GetWidth(), state->second.combo_size));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize 
        | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
         | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove 
         | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing 
         | ImGuiWindowFlags_NoScrollWithMouse;

        PushStyleColor(ImGuiCol_WindowBg, theme::colors.background);
        PushStyleVar(ImGuiStyleVar_WindowRounding, theme::metrics.rounding);
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
        PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

        bool ret = Begin(label, NULL, window_flags);

        PopStyleVar(3);
        PopStyleColor(1);

        state->second.hovered = IsWindowHovered();

        if (multi && state->second.hovered && g.IO.MouseClicked[0]) state->second.opened = false;

        return true;
    }
   
   
   
    void EndCombo()
    {
        //EndPopup();
        End();
    }

    void MultiCombo(const char *label, bool variable[], const char *labels[], int count)
    {
        ImGuiContext &g = *GImGui;
        ImGuiWindow *window = GetCurrentWindow();
        std::string preview = "None";

        for (auto i = 0, j = 0; i < count; i++)
        {
            if (variable[i])
            {
                if (j)
                    preview += (", ") + (std::string)labels[i];
                else
                    preview = labels[i];

                j++;
            }
        }
        if (BeginCombo(label, preview.c_str(), count))
        {
            for (auto i = 0; i < count; i++)
            {
                PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
                PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
                custom::Selectable(labels[i], &variable[i], ImGuiSelectableFlags_DontClosePopups);
                PopStyleVar(2);
            }
            //End();
            EndCombo();
        }

        preview = ("None");
    }

    bool BeginComboPreview()
    {
        ImGuiContext &g = *GImGui;
        ImGuiWindow *window = g.CurrentWindow;
        ImGuiComboPreviewData *preview_data = &g.ComboPreviewData;

        if (window->SkipItems || !(g.LastItemData.StatusFlags & ImGuiItemStatusFlags_Visible))
            return false;

        IM_ASSERT(g.LastItemData.Rect.Min.x == preview_data->PreviewRect.Min.x && g.LastItemData.Rect.Min.y == preview_data->PreviewRect.Min.y);

        if (!window->ClipRect.Overlaps(preview_data->PreviewRect))
            return false;

        preview_data->BackupCursorPos = window->DC.CursorPos;
        preview_data->BackupCursorMaxPos = window->DC.CursorMaxPos;
        preview_data->BackupCursorPosPrevLine = window->DC.CursorPosPrevLine;
        preview_data->BackupPrevLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
        preview_data->BackupLayout = window->DC.LayoutType;
        window->DC.CursorPos = preview_data->PreviewRect.Min+ g.Style.FramePadding;
        window->DC.CursorMaxPos = window->DC.CursorPos;
        window->DC.LayoutType = ImGuiLayoutType_Horizontal;
        window->DC.IsSameLine = false;
        PushClipRect(preview_data->PreviewRect.Min, preview_data->PreviewRect.Max, true);

        return true;
    }

    void EndComboPreview()
    {
        ImGuiContext &g = *GImGui;
        ImGuiWindow *window = g.CurrentWindow;
        ImGuiComboPreviewData *preview_data = &g.ComboPreviewData;

        ImDrawList *draw_list = window->DrawList;
        if (window->DC.CursorMaxPos.x < preview_data->PreviewRect.Max.x && window->DC.CursorMaxPos.y < preview_data->PreviewRect.Max.y)
            if (draw_list->CmdBuffer.Size > 1)
            {
                draw_list->_CmdHeader.ClipRect = draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ClipRect = draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 2].ClipRect;
                draw_list->_TryMergeDrawCmds();
            }
        PopClipRect();
        window->DC.CursorPos = preview_data->BackupCursorPos;
        window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, preview_data->BackupCursorMaxPos);
        window->DC.CursorPosPrevLine = preview_data->BackupCursorPosPrevLine;
        window->DC.PrevLineTextBaseOffset = preview_data->BackupPrevLineTextBaseOffset;
        window->DC.LayoutType = preview_data->BackupLayout;
        window->DC.IsSameLine = false;
        preview_data->PreviewRect = ImRect();
    }

    static const char *Items_ArrayGetter(void *data, int idx)
    {
        const char *const *items = (const char *const *)data;
        return items[idx];
    }

    static const char *Items_SingleStringGetter(void *data, int idx)
    {
        const char *items_separated_by_zeros = (const char *)data;
        int items_count = 0;
        const char *p = items_separated_by_zeros;
        while (*p)
        {
            if (idx == items_count)
                break;
            p += strlen(p) + 1;
            items_count++;
        }
        return *p ? p : NULL;
    }

    static float CalcMaxPopupHeightFromItemCount(int items_count)
    {
        ImGuiContext &g = *GImGui;
        if (items_count <= 0)
            return FLT_MAX;
        return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
    }

    bool Combo(const char *label, int *current_item, const char *(*getter)(void *user_data, int idx), void *user_data, int items_count, int popup_max_height_in_items)
    {
        ImGuiContext &g = *GImGui;

        const char *preview_value = NULL;
        if (*current_item >= 0 && *current_item < items_count)
            preview_value = getter(user_data, *current_item);

        if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
            SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));
        //
        if (!BeginCombo(label, preview_value, items_count, ImGuiComboFlags_None))
            return false;

        bool value_changed = false;
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
        for (int i = 0; i < items_count; i++)
        {
            const char *item_text = getter(user_data, i);
            if (item_text == NULL)
                item_text = "*Unknown item*";

            PushID(i);
            const bool item_selected = (i == *current_item);
            if (custom::Selectable(item_text, item_selected) && *current_item != i)
            {
                value_changed = true;
                *current_item = i;
            }
            if (item_selected)
                SetItemDefaultFocus();
            PopID();
        }
        PopStyleVar();

        EndCombo();

        if (value_changed)
            MarkItemEdited(g.LastItemData.ID);

        return value_changed;
    }

    bool Combo(const char *label, int *current_item, const char *const items[], int items_count, int height_in_items)
    {
        const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void *)items, items_count, height_in_items);
        return value_changed;
    }

    bool Combo(const char *label, int *current_item, const char *items_separated_by_zeros, int height_in_items)
    {
        int items_count = 0;
        const char *p = items_separated_by_zeros;
        while (*p)
        {
            p += strlen(p) + 1;
            items_count++;
        }
        bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void *)items_separated_by_zeros, items_count, height_in_items);
        return value_changed;
    }

    bool Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        ImVec2 pos = window->DC.CursorPos;
        const ImVec2 label_size = CalcTextSize(label);
        ImVec2 size(size_arg.x != 0 ? size_arg.x : label_size.x, size_arg.y != 0 ? size_arg.y : label_size.y);
        pos.y += window->DC.CurrLineTextBaseOffset;
        ItemSize(size, 0.0f);

        const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
        const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
        const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
        if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
            size.x = ImMax(label_size.x, max_x - min_x);

        const ImVec2 text_min = pos;
        const ImVec2 text_max(min_x + size.x, pos.y + size.y);

        ImRect bb(min_x, pos.y, text_max.x, text_max.y);
        if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
        {
            const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
            const float spacing_y = style.ItemSpacing.y;
            const float spacing_L = IM_TRUNC(spacing_x * 0.50f);
            const float spacing_U = IM_TRUNC(spacing_y * 0.50f);
            bb.Min.x -= spacing_L;
            bb.Min.y -= spacing_U;
            bb.Max.x += (spacing_x - spacing_L);
            bb.Max.y += (spacing_y - spacing_U);
        }

        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        if (span_all_columns)
        {
            window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
            window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        }

        const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
        const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
        
        if (span_all_columns)
        {
            window->ClipRect.Min.x = backup_clip_rect_min_x;
            window->ClipRect.Max.x = backup_clip_rect_max_x;
        }

        if (!item_add)
            return false;

        const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
        if (disabled_item && !disabled_global)
            BeginDisabled();

        if (span_all_columns && window->DC.CurrentColumns)
            PushColumnsBackground();
        else if (span_all_columns && g.CurrentTable)
            TablePushBackgroundChannel();

        ImGuiButtonFlags button_flags = 0;
        if (flags & ImGuiSelectableFlags_NoHoldingActiveID)
            button_flags |= ImGuiButtonFlags_NoHoldingActiveId;
        if (flags & ImGuiSelectableFlags_NoSetKeyOwner)
            button_flags |= ImGuiButtonFlags_NoSetKeyOwner;
        if (flags & ImGuiSelectableFlags_SelectOnClick)
            button_flags |= ImGuiButtonFlags_PressedOnClick;
        if (flags & ImGuiSelectableFlags_SelectOnRelease)
            button_flags |= ImGuiButtonFlags_PressedOnRelease;
        if (flags & ImGuiSelectableFlags_AllowDoubleClick)
            button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
        if (flags & ImGuiSelectableFlags_AllowOverlap)
            button_flags |= ImGuiButtonFlags_AllowOverlap;

        const bool was_selected = selected;
        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

        if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
        {
        if (!g.NavHighlightItemUnderNav && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
            if (g.IO.ConfigNavCursorVisibleAuto)
                g.NavCursorVisible = false;
        }
        }

        if (pressed)
            MarkItemEdited(id);

        if (selected != was_selected)
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

        if (g.NavId == id)
            RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_None);

        if (span_all_columns && window->DC.CurrentColumns)
            PopColumnsBackground();
        else if (span_all_columns && g.CurrentTable)
            TablePopBackgroundChannel();

        static std::map<ImGuiID, internal::ElementState> states;
        auto state = states.find(id);
        if (state == states.end())
        {
            states.insert({ id, internal::ElementState() });
            state = states.find(id);
        }

        state->second.text = ImLerp(
            state->second.text,
            selected ? theme::colors.text : theme::colors.text_dim,
            g.IO.DeltaTime * anim::SPEED);

        PushStyleColor(ImGuiCol_Text, GetColorU32(state->second.text));
        RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
        PopStyleColor();

        if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups))
            CloseCurrentPopup();

        if (disabled_item && !disabled_global)
            EndDisabled();

        return pressed;
    }

    bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size)
    {
        if (Selectable(label, *p_selected, flags, size))
        {
            *p_selected = !*p_selected;
            return true;
        }
        return false;
    }


    static void ColorEditRestoreH(const float* col, float* H)
    {
        ImGuiContext& g = *GImGui;
        IM_ASSERT(g.ColorEditCurrentID != 0);
        if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
            return;
        *H = g.ColorEditSavedHue;
    }

    static void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
    {
        ImGuiContext& g = *GImGui;
        IM_ASSERT(g.ColorEditCurrentID != 0);
        if (g.ColorEditSavedID != g.ColorEditCurrentID 
        || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0))) 
        return;
        // 当饱和度为0时恢复色相
        if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
            *H = g.ColorEditSavedHue;
        // 当亮度为0时恢复饱和度
        if (*V == 0.0f) *S = g.ColorEditSavedSat;
    }


    bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const float square_sz = GetFrameHeight();
         float w_full = CalcItemWidth();
        const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
        const float w_inputs = ImMax(w_full - w_button, 1.0f);
        //w_full = w_inputs + w_button;
        const char* label_display_end = FindRenderedTextEnd(label);
        g.NextItemData.ClearFlags();

        BeginGroup();
        PushID(label);

        const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
        if (set_current_color_edit_id)
            g.ColorEditCurrentID = window->IDStack.back();

        // If we're not showing any slider there's no point in doing any HSV conversions
        const ImGuiColorEditFlags flags_untouched = flags;
        if (flags & ImGuiColorEditFlags_NoInputs)
            flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

        // Context menu: display and modify options (before defaults are applied)
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            ColorEditOptionsPopup(col, flags);

    // Read stored options
    if (!(flags & ImGuiColorEditFlags_DisplayMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
    if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
    if (!(flags & ImGuiColorEditFlags_PickerMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
    if (!(flags & ImGuiColorEditFlags_InputMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
    flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ 
    | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ 
    | ImGuiColorEditFlags_InputMask_));
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_)); // Check that only 1 is selected
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected

        const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
        const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
        const int components = alpha ? 4 : 3;

        // Convert to the formats we need
        float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
        if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
        else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
        {
            // Hue is lost when converting from grayscale rgb (saturation=0). Restore it.
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
            ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
        }
        int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

        bool value_changed = false;
        bool value_changed_as_float = false;

        const ImVec2 pos = window->DC.CursorPos;
        const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
        window->DC.CursorPos.x = pos.x + inputs_offset_x;

        if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
        {
            // RGB/HSV 0..255 Sliders
            const float w_items = w_inputs - style.ItemInnerSpacing.x * (components - 1);
            const bool hide_prefix = (IM_TRUNC(w_items / components) <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
             //1.90.2
            // const float w_item_one = ImMax(1.0f, IM_FLOOR((w_inputs - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
            // const float w_item_last = ImMax(1.0f, IM_FLOOR(w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

            //const bool hide_prefix = (w_item_one <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
         
            
            static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
            static const char* fmt_table_int[3][4] =
            {
                {   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
                { "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
                { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
            };
            static const char* fmt_table_float[3][4] =
            {
                {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
                { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
                { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
            };
            const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;
            
            float prev_split = 0.0f;
            for (int n = 0; n < components; n++)
            {
                if (n > 0)
                    SameLine(0, style.ItemInnerSpacing.x);
                float next_split = IM_TRUNC(w_items * (n + 1) / components);
                SetNextItemWidth(ImMax(next_split - prev_split, 1.0f));
                prev_split = next_split;

                // FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
                if (flags & ImGuiColorEditFlags_Float)
                {
                    value_changed |= DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, 
                                hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
                    value_changed_as_float |= value_changed;
                }
                else
                {
                    value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
                }
                if (!(flags & ImGuiColorEditFlags_NoOptions))
                    OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
            }
        }
        else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
        {

            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
        }

        // RGB Hexadecimal Input
        char buf[64];
        ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X",
         ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
         
        SetNextItemWidth(w_inputs);

        if (InputText("", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
        {
            value_changed = true;
            char* p = buf;
            while (*p == '#' || ImCharIsBlankA(*p))
                p++;
            i[0] = i[1] = i[2] = 0;
            i[3] = 0xFF; // alpha default to 255 is not parsed by scanf (e.g. inputting #FFFFFF omitting alpha)
            int r;

            r = sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
            IM_UNUSED(r); // Fixes C6031: Return value ignored: 'sscanf'.
        }

        ImGuiWindow* picker_active_window = NULL;
        if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
        {
            const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs)
             || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
            window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

            const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
            if (ColorButton("##ColorButton", col_v4, flags))
            {
                if (!(flags & ImGuiColorEditFlags_NoPicker))
                {
                    g.ColorPickerRef = col_v4;
                    OpenPopup("picker");
                    const ImVec2 pos = g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y);
                    SetNextWindowPos(pos);
                }
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

            PushStyleColor(ImGuiCol_PopupBg, GetColorU32(theme::colors.background));

            PushStyleVar(ImGuiStyleVar_PopupRounding, theme::metrics.rounding);
            if (BeginPopup("picker"))
            {
                if (g.CurrentWindow->BeginCount == 1)
                {
                    picker_active_window = g.CurrentWindow;

                    ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_
                     | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ 
                     | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
                    ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward)
                    | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel 
                    | ImGuiColorEditFlags_AlphaPreviewHalf;
                    SetNextItemWidth(square_sz * 15.0f); // Use 256 + bar sizes?
                    value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
                }
                EndPopup();
            }
            PopStyleColor(1);
            PopStyleVar();
        }

        if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
        {

            static std::map<ImGuiID, internal::ElementState> states;
            auto state = states.find(ImGui::GetID(label));

            if (state == states.end())
            {
                states.insert({ ImGui::GetID(label), internal::ElementState() });
                state = states.find(ImGui::GetID(label));
            }

            SameLine(0.0f, style.ItemInnerSpacing.x);
            //window->DC.CursorPos.x = pos.x - w_button + ((flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full);

              window->DC.CursorPos.x = pos.x + ((flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full + style.ItemInnerSpacing.x);
       
            GetWindowDrawList()->AddText(
                window->DC.CursorPos + ImVec2(0, (32 - CalcTextSize(buf).y) / 2), 
                GetColorU32(theme::colors.text),  label);

        }

        // Convert back
        if (value_changed && picker_active_window == NULL)
        {
            if (!value_changed_as_float)
                for (int n = 0; n < 4; n++)
                    f[n] = i[n] / 255.0f;
            if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
            {
                g.ColorEditSavedHue = f[0];
                g.ColorEditSavedSat = f[1];
                ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
                g.ColorEditSavedID = g.ColorEditCurrentID;
                g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
            }
            if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
                ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

            col[0] = f[0];
            col[1] = f[1];
            col[2] = f[2];
            if (alpha)
                col[3] = f[3];
        }

        if (set_current_color_edit_id)
            g.ColorEditCurrentID = 0;
        PopID();
        EndGroup();

        // Drag and Drop Target
        // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
        if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget())
        {
            bool accepted_drag_drop = false;
            if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
            {
                memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512 //-V1086
                value_changed = accepted_drag_drop = true;
            }
            if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
            {
                memcpy((float*)col, payload->Data, sizeof(float) * components);
                value_changed = accepted_drag_drop = true;
            }

            // Drag-drop payloads are always RGB
            if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
                ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
            EndDragDropTarget();
        }

        // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
        if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
            g.LastItemData.ID = g.ActiveId;

        if (value_changed && g.LastItemData.ID != 0) // In case of ID collision, the second EndGroup() won't catch g.ActiveId
            MarkItemEdited(g.LastItemData.ID);

        return value_changed;
    }

bool ColorEdit42(const char* label, float col[4], ImGuiColorEditFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float square_sz = GetFrameHeight();
    const char* label_display_end = FindRenderedTextEnd(label);
    float w_full = CalcItemWidth();
    g.NextItemData.ClearFlags();

    BeginGroup();
    PushID(label);
    const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
    if (set_current_color_edit_id)
        g.ColorEditCurrentID = window->IDStack.back();

    // If we're not showing any slider there's no point in doing any HSV conversions
    const ImGuiColorEditFlags flags_untouched = flags;
    if (flags & ImGuiColorEditFlags_NoInputs)
        flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

    // Context menu: display and modify options (before defaults are applied)
    if (!(flags & ImGuiColorEditFlags_NoOptions))
        ColorEditOptionsPopup(col, flags);

    // Read stored options
    if (!(flags & ImGuiColorEditFlags_DisplayMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
    if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
    if (!(flags & ImGuiColorEditFlags_PickerMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
    if (!(flags & ImGuiColorEditFlags_InputMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
    flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_)); // Check that only 1 is selected
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected

    const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
    const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
    const int components = alpha ? 4 : 3;
    const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
    const float w_inputs = ImMax(w_full - w_button, 1.0f);
    w_full = w_inputs + w_button;

    // Convert to the formats we need
    float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
    if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
        ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
    else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
    {
        // Hue is lost when converting from grayscale rgb (saturation=0). Restore it.
        ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
        ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
    }
    int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

    bool value_changed = false;
    bool value_changed_as_float = false;

    const ImVec2 pos = window->DC.CursorPos;
    const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
    window->DC.CursorPos.x = pos.x + inputs_offset_x;

    if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
    {
        // RGB/HSV 0..255 Sliders
        const float w_items = w_inputs - style.ItemInnerSpacing.x * (components - 1);

        const bool hide_prefix = (IM_TRUNC(w_items / components) <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
        static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
        static const char* fmt_table_int[3][4] =
        {
            {   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
            { "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
            { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
        };
        static const char* fmt_table_float[3][4] =
        {
            {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
            { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
            { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
        };
        const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

        float prev_split = 0.0f;
        for (int n = 0; n < components; n++)
        {
            if (n > 0)
                SameLine(0, style.ItemInnerSpacing.x);
            float next_split = IM_TRUNC(w_items * (n + 1) / components);
            SetNextItemWidth(ImMax(next_split - prev_split, 1.0f));
            prev_split = next_split;

            // FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
            if (flags & ImGuiColorEditFlags_Float)
            {
                value_changed |= DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
                value_changed_as_float |= value_changed;
            }
            else
            {
                value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
        }
    }
    else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
    {
        // RGB Hexadecimal Input
        char buf[64];
        if (alpha)
            ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
        else
            ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
        SetNextItemWidth(w_inputs);
        if (InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsUppercase))
        {
            value_changed = true;
            char* p = buf;
            while (*p == '#' || ImCharIsBlankA(*p))
                p++;
            i[0] = i[1] = i[2] = 0;
            i[3] = 0xFF; // alpha default to 255 is not parsed by scanf (e.g. inputting #FFFFFF omitting alpha)
            int r;
            if (alpha)
                r = sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
            else
                r = sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
            IM_UNUSED(r); // Fixes C6031: Return value ignored: 'sscanf'.
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
    }

    ImGuiWindow* picker_active_window = NULL;
    if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
    {
        const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs)
         || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
        window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

        const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
        if (ColorButton("##ColorButton", col_v4, flags))
        {
            if (!(flags & ImGuiColorEditFlags_NoPicker))
            {
                // Store current color and open a picker
                g.ColorPickerRef = col_v4;
                OpenPopup("picker");
                SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y));
            }
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

        if (BeginPopup("picker"))
        {
            if (g.CurrentWindow->BeginCount == 1)
            {
                picker_active_window = g.CurrentWindow;
                if (label != label_display_end)
                {
                    TextEx(label, label_display_end);
                    Spacing();
                }
                ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
                ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
                SetNextItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
                value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
            }
            EndPopup();
        }
    }

    if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
    {
        // Position not necessarily next to last submitted button (e.g. if style.ColorButtonPosition == ImGuiDir_Left),
        // but we need to use SameLine() to setup baseline correctly. Might want to refactor SameLine() to simplify this.
        SameLine(0.0f, style.ItemInnerSpacing.x);
        window->DC.CursorPos.x = pos.x + ((flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full + style.ItemInnerSpacing.x);
        TextEx(label, label_display_end);
    }

    // Convert back
    if (value_changed && picker_active_window == NULL)
    {
        if (!value_changed_as_float)
            for (int n = 0; n < 4; n++)
                f[n] = i[n] / 255.0f;
        if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
        {
            g.ColorEditSavedHue = f[0];
            g.ColorEditSavedSat = f[1];
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
            g.ColorEditSavedID = g.ColorEditCurrentID;
            g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
        }
        if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

        col[0] = f[0];
        col[1] = f[1];
        col[2] = f[2];
        if (alpha)
            col[3] = f[3];
    }

    if (set_current_color_edit_id)
        g.ColorEditCurrentID = 0;
    PopID();
    EndGroup();

    // Drag and Drop Target
    // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
    if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(g.LastItemData.ItemFlags & ImGuiItemFlags_ReadOnly) && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget())
    {
        bool accepted_drag_drop = false;
        if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512 //-V1086
            value_changed = accepted_drag_drop = true;
        }
        if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * components);
            value_changed = accepted_drag_drop = true;
        }

        // Drag-drop payloads are always RGB
        if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
        EndDragDropTarget();
    }

    // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
    if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
        g.LastItemData.ID = g.ActiveId;

    if (value_changed && g.LastItemData.ID != 0) // In case of ID collision, the second EndGroup() won't catch g.ActiveId
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}


    // Helper for ColorPicker4()
    static void RenderArrowsForVerticalBar(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w, float alpha)
    {
        ImU32 alpha8 = IM_F32_TO_INT8_SAT(alpha);
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x + 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Right, IM_COL32(0, 0, 0, alpha8));
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x, pos.y), half_sz, ImGuiDir_Right, IM_COL32(255, 255, 255, alpha8));
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Left, IM_COL32(0, 0, 0, alpha8));
        ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x, pos.y), half_sz, ImGuiDir_Left, IM_COL32(255, 255, 255, alpha8));
    }

    bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImDrawList* draw_list = window->DrawList;
        ImGuiStyle& style = g.Style;
        ImGuiIO& io = g.IO;

        const float width = CalcItemWidth();
        g.NextItemData.ClearFlags();

        PushID(label);
        BeginGroup();

        if (!(flags & ImGuiColorEditFlags_NoSidePreview))
            flags |= ImGuiColorEditFlags_NoSmallPreview;

        if (!(flags & ImGuiColorEditFlags_NoOptions))
            ColorPickerOptionsPopup(col, flags);

        // Read stored options
        if (!(flags & ImGuiColorEditFlags_PickerMask_))
            flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
        if (!(flags & ImGuiColorEditFlags_InputMask_))
            flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_InputMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_)); // Check that only 1 is selected
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);

        // Setup
        int components = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
        bool alpha_bar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
        ImVec2 picker_pos = window->DC.CursorPos;
        ImVec2 bar_pos = window->DC.CursorPos + ImVec2(0, 133);
        float square_sz = GetFrameHeight();
        float bars_width = 209.f; // Arbitrary smallish width of Hue/Alpha picking bars
        float sv_picker_size = ImMax(bars_width * 1, width - (alpha_bar ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)) + 0; // Saturation/Value picking box
        float sv_bar_size = 20; // Saturation/Value picking box
        float bar0_pos_x = GetWindowPos().x + style.WindowPadding.x;
        float bar1_pos_x = bar0_pos_x;
        float bars_triangles_half_sz = IM_FLOOR(bars_width * 0.20f);

        float backup_initial_col[4];
        memcpy(backup_initial_col, col, components * sizeof(float));

        float H = col[0], S = col[1], V = col[2];
        float R = col[0], G = col[1], B = col[2];
        if (flags & ImGuiColorEditFlags_InputRGB)
        {
            // Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
            ColorConvertRGBtoHSV(R, G, B, H, S, V);
            ColorEditRestoreHS(col, &H, &S, &V);
        }
        else if (flags & ImGuiColorEditFlags_InputHSV)
        {
            ColorConvertHSVtoRGB(H, S, V, R, G, B);
        }

        bool value_changed = false, value_changed_h = false, value_changed_sv = false;

        PushItemFlag(ImGuiItemFlags_NoNav, true);

        // SV rectangle logic
        InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size - 80));
        if (IsItemActive())
        {
            S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
            V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 80));

            // Greatly reduces hue jitter and reset to 0 when hue == 255 and color is rapidly modified using SV square.
            if (g.ColorEditSavedColor == ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
                H = g.ColorEditSavedHue;
            value_changed = value_changed_sv = true;
        }

        // Hue bar logic
        SetCursorScreenPos(ImVec2(bar0_pos_x, bar_pos.y));
        InvisibleButton("hue", ImVec2(bars_width, sv_bar_size));
        if (IsItemActive())
        {
            H = 1.f - ImSaturate((io.MousePos.x - bar_pos.x) / (bars_width - 1));
            value_changed = value_changed_h = true;
        }

        // Alpha bar logic
        if (alpha_bar)
        {
            SetCursorScreenPos(ImVec2(bar1_pos_x, bar_pos.y + 16));
            InvisibleButton("alpha", ImVec2(bars_width, sv_bar_size));
            if (IsItemActive())
            {
                col[3] = ImSaturate((io.MousePos.x - bar_pos.x) / (bars_width - 1));
                value_changed = true;
            }
        }
        PopItemFlag(); // ImGuiItemFlags_NoNav

        // Convert back color to RGB
        if (value_changed_h || value_changed_sv)
        {
            if (flags & ImGuiColorEditFlags_InputRGB)
            {
                ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
                g.ColorEditSavedHue = H;
                g.ColorEditSavedSat = S;
                g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));
            }

            else if (flags & ImGuiColorEditFlags_InputHSV)
            {
                col[0] = H;
                col[1] = S;
                col[2] = V;
            }
        }

        // R,G,B and H,S,V slider color editor
        bool value_changed_fix_hue_wrap = false;

        if (value_changed_fix_hue_wrap && (flags & ImGuiColorEditFlags_InputRGB))
        {
            // Try to cancel hue wrap (after ColorEdit4 call), if any
            float new_H, new_S, new_V;
            ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
            if (new_H <= 0 && H > 0)
            {
                if (new_V <= 0 && V != new_V)
                    ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
                else if (new_S <= 0)
                    ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
            }
        }

        if (value_changed)
        {
            if (flags & ImGuiColorEditFlags_InputRGB)
            {
                R = col[0];
                G = col[1];
                B = col[2];
                ColorConvertRGBtoHSV(R, G, B, H, S, V);
                ColorEditRestoreHS(col, &H, &S, &V);   // Fix local Hue as display below will use it immediately.
            }
            else if (flags & ImGuiColorEditFlags_InputHSV)
            {
                H = col[0];
                S = col[1];
                V = col[2];
                ColorConvertHSVtoRGB(H, S, V, R, G, B);
            }
        }
        ImU32 user_col32_striped_of_alpha = ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha)); // Important: this is still including the main rendering/style alpha!!

        const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
        const ImU32 col_black = IM_COL32(0, 0, 0, style_alpha8);
        const ImU32 col_white = IM_COL32(255, 255, 255, style_alpha8);
        const ImU32 col_midgrey = IM_COL32(128, 128, 128, style_alpha8);
        const ImU32 col_hues[7] = { IM_COL32(255,0,0,style_alpha8), 
        IM_COL32(255,0,255,style_alpha8), IM_COL32(0,0,255,style_alpha8),
        IM_COL32(0,255,255,style_alpha8), IM_COL32(0,255,0,style_alpha8), 
        IM_COL32(255,255,0,style_alpha8), IM_COL32(255,0,0,style_alpha8) };

        ImVec4 hue_color_f(1, 1, 1, style.Alpha); 
        ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
        ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);

        ImVec2 sv_cursor_pos;

        // Render SV Square
        const int vtx_idx_0 = draw_list->VtxBuffer.Size;
        draw_list->AddRectFilled(picker_pos, 
        picker_pos + ImVec2(sv_picker_size, sv_picker_size - 2 - 80),
         col_white, 4.0f);
        const int vtx_idx_1 = draw_list->VtxBuffer.Size;
        ShadeVertsLinearColorGradientKeepAlpha(draw_list, vtx_idx_0, vtx_idx_1,
         picker_pos, picker_pos + ImVec2(sv_picker_size, 0.0f), col_white, hue_color32);

        draw_list->AddRectFilledMultiColor(picker_pos, 
        picker_pos + ImVec2(sv_picker_size, sv_picker_size - 80), 
        0, 0, col_black, col_black);//, 4.0f

        sv_cursor_pos.x = ImClamp(IM_ROUND(picker_pos.x + ImSaturate(S) * sv_picker_size), picker_pos.x, picker_pos.x + sv_picker_size - 2); // Sneakily prevent the circle to stick out too much
        sv_cursor_pos.y = ImClamp(IM_ROUND(picker_pos.y + ImSaturate(1 - V) * (sv_picker_size - 80)), picker_pos.y + 2, picker_pos.y + sv_picker_size - 80);

        static std::map<ImGuiID, internal::ColorPickerState> anim;
        auto it_anim = anim.find(ImGui::GetID(label));

        if (it_anim == anim.end())
        {
            anim.insert({ ImGui::GetID(label), internal::ColorPickerState() });
            it_anim = anim.find(ImGui::GetID(label));
        }

        for (int i = 0; i < 6; ++i)
            GetForegroundDrawList()->AddRectFilledMultiColor(
                ImVec2(picker_pos.x + i * (bars_width / 6) - (i == 5 ? 1 : 0),
             picker_pos.y + 139), 
            ImVec2(picker_pos.x + (i + 1) * (bars_width / 6) + (i == 0 ? 1 : 0), picker_pos.y + 132 + sv_bar_size - 7),
             col_hues[i], col_hues[i + 1], col_hues[i + 1], col_hues[i]
             );

        float bar0_line_x = IM_ROUND(bar_pos.x + (1.f - H) * bars_width);

        bar0_line_x = ImClamp(bar0_line_x, picker_pos.x + 3.f, picker_pos.x + 204.f);

        it_anim->second.hue_bar = ImLerp(it_anim->second.hue_bar, bar0_line_x - bar_pos.x, g.IO.DeltaTime * 24.f);

        GetForegroundDrawList()->AddCircleFilled(
            ImVec2(it_anim->second.hue_bar + bar_pos.x, bar_pos.y + 9),
             6.5f, ImColor(255, 255, 255, 255), 30.f);

        it_anim->second.circle_move = ImLerp(
            it_anim->second.circle_move, 
            sv_cursor_pos - bar_pos, g.IO.DeltaTime * 24.f);

        it_anim->second.circle = ImLerp(
            it_anim->second.circle,
             value_changed_sv ? 4.f : 7.f, g.IO.DeltaTime * 24.f);

        GetForegroundDrawList()->AddCircle(
            it_anim->second.circle_move + bar_pos + ImVec2(0, 1),
             it_anim->second.circle, ImColor(255, 255, 255, 255), 32);

        if (alpha_bar)
        {
            float alpha = ImSaturate(col[3]);
            ImRect bar1_bb(bar1_pos_x, bar_pos.y + 20, bar1_pos_x + bars_width, bar_pos.y + 20 + sv_bar_size);

            draw_list->AddRectFilledMultiColor(picker_pos + ImVec2(0, 161), 
            picker_pos + ImVec2(bars_width, 147 + sv_bar_size), 
            col_black, user_col32_striped_of_alpha, 
            user_col32_striped_of_alpha, col_black);

            float bar1_line_x = IM_ROUND(bar_pos.x + alpha * bars_width);

            bar1_line_x = ImClamp(bar1_line_x, bar_pos.x, picker_pos.x + 200.f);
            it_anim->second.alpha_bar = ImLerp(
                it_anim->second.alpha_bar, bar1_line_x - bar_pos.x + 5.f,
                 g.IO.DeltaTime * 24.f);
            GetForegroundDrawList()->AddCircleFilled(
                ImVec2(it_anim->second.alpha_bar + bar_pos.x, bar1_bb.Min.y + 11.0f),
                 6.5f, ImColor(255, 255, 255, 255), 100.f);
        }

        EndGroup();

        if (value_changed && memcmp(backup_initial_col, col, components * sizeof(float)) == 0) value_changed = false;
        if (value_changed) MarkItemEdited(g.LastItemData.ID);

        PopID();
        return value_changed;
    }



    bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiID id = window->GetID(desc_id);
        const float default_size = GetFrameHeight();
        const ImVec2 pos = window->DC.CursorPos;
        const float width = GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x;
        const ImRect rect(pos,  pos + ImVec2(width, 32));

        const ImRect clickable(rect.Min + ImVec2(width - 47, 7), 
                               rect.Max - ImVec2(7, 7));

        ItemSize(ImRect(rect.Min, rect.Max - ImVec2(0, 0)));
        if (!ItemAdd(rect, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

        if (flags & ImGuiColorEditFlags_NoAlpha) flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);

        ImVec4 col_rgb = col;
        if (flags & ImGuiColorEditFlags_InputHSV) ColorConvertHSVtoRGB(col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z);

        GetWindowDrawList()->AddRectFilled(
            clickable.Min, clickable.Max,
             GetColorU32(col_rgb), theme::metrics.rounding);

        RenderColorRectWithAlphaCheckerboard(window->DrawList,
         clickable.Min, clickable.Max, 
        GetColorU32(col_rgb), ImMin(36, 29) / 2.99f, 
        ImVec2(0.f, 0.f), theme::metrics.rounding);


        return pressed;
    }


    bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const float w = GetContentRegionMax().x - style.WindowPadding.x;
        const ImVec2 pos = window->DC.CursorPos;
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const ImRect frame_bb(pos, pos + ImVec2(w, 50));

        const ImRect slider_bb(pos + ImVec2(0, 40), pos + ImVec2(w, 45));

        const ImRect total_bb(frame_bb.Min,
         frame_bb.Max + ImVec2(label_size.x > 0.0f ? label_size.x : 0.0f, 0.0f));

        const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
        ItemSize(ImRect(total_bb.Min, total_bb.Max - ImVec2(0, 18)));

        if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0)) return false;

        if (format == NULL) format = DataTypeGetInfo(data_type)->PrintFmt;

        bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.ItemFlags),
         held, pressed = ButtonBehavior(frame_bb, id, &hovered, &held, NULL);

        ImRect grab_bb;

        static std::map<ImGuiID, internal::SliderState> states;
        auto state = states.find(id);

        if (state == states.end())
        {
            states.insert({ id, internal::SliderState() });
            state = states.find(id);
        }

        state->second.text = ImLerp(state->second.text, 
        g.ActiveId == id ? theme::colors.text_active : hovered ? theme::colors.text_hov : theme::colors.text,
         g.IO.DeltaTime * anim::SPEED);

        const bool value_changed = SliderBehavior(ImRect(frame_bb.Min- ImVec2(4, 0), 
                    frame_bb.Max + ImVec2(10, 0)), 
                    id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);

        GetWindowDrawList()->AddRectFilled(slider_bb.Min, slider_bb.Max,
         GetColorU32(theme::colors.background), 30 , ImDrawFlags_RoundCornersTop);

        if (value_changed) MarkItemEdited(id);

        char value_buf[64];
        const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

        state->second.slow = ImLerp(state->second.slow, grab_bb.Min.x - (frame_bb.Min.x), g.IO.DeltaTime * 25.f);

        PushClipRect(slider_bb.Min + ImVec2(1, 0), slider_bb.Max, true);

        GetWindowDrawList()->AddRectFilled(slider_bb.Min + ImVec2(0, 0), 
                ImVec2(state->second.slow + (slider_bb.Min.x + 2), grab_bb.Max.y - 3), 
        GetColorU32(theme::colors.accent), 30.f, ImDrawFlags_RoundCornersTop);
        PopClipRect();

        GetWindowDrawList()->AddText(ImVec2(frame_bb.Max.x - CalcTextSize(value_buf).x, 
        frame_bb.Min.y + (32 - CalcTextSize(value_buf).y) / 2), 
        GetColorU32(theme::colors.text), value_buf);
        GetWindowDrawList()->AddText(ImVec2(frame_bb.Max.x - w, 
        frame_bb.Min.y + (32 - CalcTextSize(value_buf).y) / 2), 
        GetColorU32(state->second.text), label);

        return value_changed;
    }

    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
    }

    bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
    {
        return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
    }



    bool ThemeButton(const char* id_theme, bool dark, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();

        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(id_theme);
        const ImVec2 label_size = CalcTextSize(id_theme, NULL, true), pos = window->DC.CursorPos;

        ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

        const ImRect bb(pos, pos+ size);

        ItemSize(size, 0.f);
        if (!ItemAdd(bb, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);

        static std::map<ImGuiID, internal::theme_state> states;
        auto it_anim = states.find(id);

        if (it_anim == states.end())
        {
            states.insert({ id, internal::theme_state() });
            it_anim = states.find(id);
        }

        it_anim->second.background = ImLerp(it_anim->second.background,
         dark || hovered ? theme::colors.background_light : theme::colors.background, g.IO.DeltaTime * anim::SPEED);

        it_anim->second.alpha_line = ImLerp(it_anim->second.alpha_line,
         dark ? 1.f : 0.f, g.IO.DeltaTime * anim::SPEED);
        it_anim->second.line_size = ImLerp(it_anim->second.line_size,
         dark ? (size_arg.x / 4) : (size_arg.x / 2), g.IO.DeltaTime * anim::SPEED);

        it_anim->second.smooth_swap = ImLerp(it_anim->second.smooth_swap,
         dark ? 26.f : 0, g.IO.DeltaTime * anim::SPEED * 2);

        GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max,
         GetColorU32(it_anim->second.background), theme::metrics.rounding);

        PushClipRect(bb.Min, bb.Max, true);

        PushFont(font::icomoon_page_);
         ImVec2 text_pos = CalcTextSize("k");
        GetWindowDrawList()->AddText(ImVec2(bb.Min.x + (size_arg.x - text_pos.x) / 2,
         bb.Max.y - text_pos.y - (size.y - text_pos.y) / 2 + it_anim->second.smooth_swap), 
         GetColorU32(theme::colors.accent), "k");

        text_pos = CalcTextSize("a");
        GetWindowDrawList()->AddText(ImVec2(bb.Min.x + (size_arg.x - text_pos.x) / 2,
         bb.Max.y - text_pos.y - (size.y - text_pos.y) / 2 - 25 + it_anim->second.smooth_swap), 
         GetColorU32(theme::colors.accent), "a");
        PopFont();

        PopClipRect();


        return pressed;
    }


    void Separator_line()
    {
        GetWindowDrawList()->AddRectFilled(GetCursorScreenPos(),
         GetCursorScreenPos()+ ImVec2(GetContentRegionMax().x - GetStyle().WindowPadding.x, 1), 
         GetColorU32(theme::colors.separator));
        Spacing();
    }

    void SeparatorEx(ImGuiSeparatorFlags flags, float thickness)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return;

        ImGuiContext& g = *GImGui;
        IM_ASSERT(ImIsPowerOfTwo(flags & (ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical)));
        IM_ASSERT(thickness > 0.0f);

        if (flags & ImGuiSeparatorFlags_Vertical)
        {
            float y1 = window->DC.CursorPos.y;
            float y2 = window->DC.CursorPos.y + window->DC.CurrLineSize.y;
            const ImRect bb(ImVec2(window->DC.CursorPos.x, y1 + (GetStyle().ItemSpacing.y / 2)), ImVec2(window->DC.CursorPos.x + thickness, y2 - (GetStyle().ItemSpacing.y / 2)));


            ItemSize(ImVec2(thickness, 0.0f));
            if (!ItemAdd(bb, 0)) return;

            window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(theme::colors.background));

            ImGui::SameLine();
        }
        else if (flags & ImGuiSeparatorFlags_Horizontal)
        {
            float x1 = window->Pos.x;
            float x2 = window->Pos.x + window->Size.x;

            if (g.GroupStack.Size > 0 && g.GroupStack.back().WindowID == window->ID) x1 += window->DC.Indent.x;

            if (ImGuiTable* table = g.CurrentTable)
            {
                x1 = table->Columns[table->CurrentColumn].MinX;
                x2 = table->Columns[table->CurrentColumn].MaxX;
            }

            ImGuiOldColumns* columns = (flags & ImGuiSeparatorFlags_SpanAllColumns) ? window->DC.CurrentColumns : NULL;
            if (columns) PushColumnsBackground();

            const float thickness_for_layout = (thickness == 1.0f) ? 0.0f : thickness;
            const ImRect bb(ImVec2(x1 + GetStyle().WindowPadding.x, window->DC.CursorPos.y), ImVec2(x2 - GetStyle().WindowPadding.x, window->DC.CursorPos.y + thickness));

            ItemSize(ImVec2(0.0f, thickness_for_layout));

            if (ItemAdd(bb, 0))
            {
                window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(theme::colors.separator));
            }
            if (columns)
            {
                PopColumnsBackground();
                columns->LineMinY = window->DC.CursorPos.y;
            }
        }
    }

    void Separator()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        if (window->SkipItems) return;

        ImGuiSeparatorFlags flags = (window->DC.LayoutType == ImGuiLayoutType_Horizontal) ? ImGuiSeparatorFlags_Vertical : ImGuiSeparatorFlags_Horizontal;
        flags |= ImGuiSeparatorFlags_SpanAllColumns;
        SeparatorEx(flags, 1.0f);
    }

  






} // namespace custom
