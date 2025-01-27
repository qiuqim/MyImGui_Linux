#include "theme.hpp"

namespace theme {
    Theme current;
    TransitionConfig transition_config;

    namespace presets {
        Theme light = {
            // Base colors
            {
                ImColor(121, 131, 207),     // accent
                ImColor(255, 255, 255),     // background
                ImColor(222, 228, 244),     // separator
            },
            // Text colors
            {
                ImColor(0, 0, 0),           // primary
                ImColor(68, 71, 81),        // secondary
                ImColor(0, 0, 0),           // active
                ImColor(68, 71, 81),        // hovered
                ImColor(128, 128, 128),     // disabled
            },
            // Elements colors
            {
                ImColor(222, 228, 244),     // background
                ImColor(197, 207, 232),     // background_hovered
                ImColor(196, 205, 228),     // background_active
                ImColor(200, 200, 200),     // border
                ImColor(150, 150, 150),     // border_hovered
                ImColor(100, 100, 100),     // border_active
            },
            // Child colors
            {
                ImColor(241, 243, 249),     // background
                ImColor(228, 235, 248),     // header
                ImColor(222, 228, 244),     // border
            },
            // Page colors
            {
                ImColor(222, 228, 244),     // background
                ImColor(196, 205, 228),     // background_active
                ImColor(136, 145, 176),     // text
                ImColor(136, 145, 176),     // text_hovered
            },
            // Special colors
            {
                ImColor(255, 255, 255),     // checkbox_mark
                ImColor(121, 131, 207),     // slider_grab
                ImColor(100, 110, 186),     // button_active
                ImColor(241, 243, 249),     // notification_bg
                ImColor(50, 50, 50),        // tooltip_bg
            }
        };

        Theme dark = {
            // Base colors
            {
                ImColor(118, 187, 117),     // accent
                ImColor(15, 15, 15),        // background
                ImColor(22, 23, 26),        // separator
            },
            // Text colors
            {
                ImColor(255, 255, 255),     // primary
                ImColor(68, 71, 85),        // secondary
                ImColor(255, 255, 255),     // active
                ImColor(68, 71, 85),        // hovered
                ImColor(128, 128, 128),     // disabled
            },
            // Elements colors
            {
                ImColor(22, 23, 25),        // background
                ImColor(31, 33, 38),        // background_hovered
                ImColor(31, 33, 38),        // background_active
                ImColor(40, 40, 40),        // border
                ImColor(60, 60, 60),        // border_hovered
                ImColor(80, 80, 80),        // border_active
            },
            // Child colors
            {
                ImColor(17, 17, 18),        // background
                ImColor(20, 21, 23),        // header
                ImColor(22, 23, 26),        // border
            },
            // Page colors
            {
                ImColor(22, 23, 25),        // background
                ImColor(31, 33, 38),        // background_active
                ImColor(68, 71, 85),        // text
                ImColor(68, 71, 85),        // text_hovered
            },
            // Special colors
            {
                ImColor(0, 0, 0),           // checkbox_mark
                ImColor(118, 187, 117),     // slider_grab
                ImColor(98, 167, 97),       // button_active
                ImColor(17, 17, 18),        // notification_bg
                ImColor(35, 35, 35),        // tooltip_bg
            }
        };
    }

    void UpdateTheme(bool is_dark, float delta_time) {
        if (!transition_config.enable_transitions) {
            current = is_dark ? presets::dark : presets::light;
            return;
        }

        const Theme& target = is_dark ? presets::dark : presets::light;

        // 基础颜色过渡
        current.base.accent = LerpColor(current.base.accent, target.base.accent, delta_time * transition_config.base_speed);
        current.base.background = LerpColor(current.base.background, target.base.background, delta_time * transition_config.base_speed);
        current.base.separator = LerpColor(current.base.separator, target.base.separator, delta_time * transition_config.base_speed);

        // 文本颜色过渡
        current.text.primary = LerpColor(current.text.primary, target.text.primary, delta_time * transition_config.base_speed);
        current.text.secondary = LerpColor(current.text.secondary, target.text.secondary, delta_time * transition_config.base_speed);
        current.text.active = LerpColor(current.text.active, target.text.active, delta_time * transition_config.base_speed);
        current.text.hovered = LerpColor(current.text.hovered, target.text.hovered, delta_time * transition_config.base_speed);
        current.text.disabled = LerpColor(current.text.disabled, target.text.disabled, delta_time * transition_config.base_speed);

        // 控件颜色过渡
        current.elements.background = LerpColor(current.elements.background, target.elements.background, delta_time * transition_config.elements_speed);
        current.elements.background_hovered = LerpColor(current.elements.background_hovered, target.elements.background_hovered, delta_time * transition_config.elements_speed);
        current.elements.background_active = LerpColor(current.elements.background_active, target.elements.background_active, delta_time * transition_config.elements_speed);
        current.elements.border = LerpColor(current.elements.border, target.elements.border, delta_time * transition_config.elements_speed);
        current.elements.border_hovered = LerpColor(current.elements.border_hovered, target.elements.border_hovered, delta_time * transition_config.elements_speed);
        current.elements.border_active = LerpColor(current.elements.border_active, target.elements.border_active, delta_time * transition_config.elements_speed);

        // 子窗口颜色过渡
        current.child.background = LerpColor(current.child.background, target.child.background, delta_time * transition_config.base_speed);
        current.child.header = LerpColor(current.child.header, target.child.header, delta_time * transition_config.base_speed);
        current.child.border = LerpColor(current.child.border, target.child.border, delta_time * transition_config.base_speed);

        // 页面颜色过渡
        current.page.background = LerpColor(current.page.background, target.page.background, delta_time * transition_config.elements_speed);
        current.page.background_active = LerpColor(current.page.background_active, target.page.background_active, delta_time * transition_config.elements_speed);
        current.page.text = LerpColor(current.page.text, target.page.text, delta_time * transition_config.base_speed);
        current.page.text_hovered = LerpColor(current.page.text_hovered, target.page.text_hovered, delta_time * transition_config.base_speed);

        // 特殊控件颜色过渡
        current.special.checkbox_mark = LerpColor(current.special.checkbox_mark, target.special.checkbox_mark, delta_time * transition_config.base_speed);
        current.special.slider_grab = LerpColor(current.special.slider_grab, target.special.slider_grab, delta_time * transition_config.base_speed);
        current.special.button_active = LerpColor(current.special.button_active, target.special.button_active, delta_time * transition_config.base_speed);
        current.special.notification_bg = LerpColor(current.special.notification_bg, target.special.notification_bg, delta_time * transition_config.base_speed);
        current.special.tooltip_bg = LerpColor(current.special.tooltip_bg, target.special.tooltip_bg, delta_time * transition_config.base_speed);
    }
} 