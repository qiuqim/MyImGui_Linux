
#define IMGUI_DEFINE_MATH_OPERATORS 
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imgui_freetype.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <functional>
#include "stb_image.h"

#include "custom_widgets.hpp"
#include "font.h"
#include "notify.h"
#include "image.h"




struct BaseTexData {
    void *DS = nullptr;
    int Width = 0;
    int Height = 0;
    int Channels = 0;

    BaseTexData() = default;

    BaseTexData(BaseTexData &other) = default;
};

struct TextureInfo {
    unsigned long long DS = 0;
    int w;
    int h;
};

TextureInfo preview_slow_ {};

BaseTexData *LoadTexture(BaseTexData *tex, void *pixel_data) {
    auto tex_data = new BaseTexData();
    tex_data->Width = tex->Width;
    tex_data->Height = tex->Height;
    tex_data->Channels = tex->Channels;

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_data->Width, tex_data->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
    tex_data->DS = (void *) (intptr_t) textureId;
    return tex_data;
}

TextureInfo LoadTextureData(const std::function<unsigned char *(BaseTexData *)> &loadFunc) {
    TextureInfo ret_data{0};
    BaseTexData tex_data{};

    tex_data.Channels = 4;
    unsigned char *image_data = loadFunc(&tex_data);
    if (image_data == nullptr)
        return ret_data;

    auto result = LoadTexture(&tex_data, image_data);

    stbi_image_free(image_data);

    ret_data.DS = (unsigned long long)result->DS;
    ret_data.w = result->Width;
    ret_data.h = result->Height;
    //delete(result);
    return ret_data;
}

TextureInfo LoadTextureFromMemory(void *data, int len) {
    return LoadTextureData([data, len](BaseTexData *tex_data) {
        return stbi_load_from_memory((const stbi_uc *) data, len, &tex_data->Width, &tex_data->Height, nullptr, tex_data->Channels);
    });
}

void ShowCustomWidgetsDemo()
{
        ImGui::SetNextWindowSizeConstraints(ImVec2(bg::size.x, bg::size.y), ImGui::GetIO().DisplaySize);
        Begin("M1LL3X", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
        {
            ImGuiStyle* style = &ImGui::GetStyle();

            const ImVec2& pos = ImGui::GetWindowPos();
            const ImVec2& region = ImGui::GetContentRegionMax();
            const ImVec2& spacing = style->ItemSpacing;

            style->WindowPadding = ImVec2(0, 0);
            style->ItemSpacing = ImVec2(20, 20);
            style->WindowBorderSize = 0;
            style->ScrollbarSize = 10.f;

            //更新颜色配置
            theme::UpdateTheme(dark, ImGui::GetIO().DeltaTime);

            //全屏背景颜色
            GetWindowDrawList()->AddRectFilled(pos, pos + ImVec2(region),
             ImGui::GetColorU32(theme::current.child.background.Value), custom::internal::rounding);
            GetWindowDrawList()->AddRectFilled(pos + spacing, 
                        pos + ImVec2(region.x - spacing.x, 50 + spacing.y),
             ImGui::GetColorU32(theme::current.child.background.Value), custom::internal::rounding);


            GetWindowDrawList()->AddImage((ImTextureID)preview_slow_.DS,
             pos + ImVec2(region.x - (spacing.x + 89), spacing.y),
              pos + ImVec2(region.x - spacing.x, 50 + spacing.y), 
              ImVec2(0, 0), ImVec2(1, 1),
             ImGui::GetColorU32(theme::current.base.accent.Value, 0.6f));


        //   PushFont(font::icomoon_logo);
        //   GetWindowDrawList()->AddText(pos + ImVec2(region.x - (spacing.x + CalcTextSize("m").x) - 14, 50 - (spacing.y + CalcTextSize("m").y) + 31), GetColorU32(c::accent, 1.f), "m");
        //   PopFont();
        //
            SetCursorPos(ImVec2(spacing.x, (50 + (spacing.y * 2))));

            tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (page == active_tab ? 1.f : -1.f)), 0.f, 1.f);
            if (tab_alpha == 0.f && tab_add == 0.f) active_tab = page;

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);

            if (active_tab == 0)
            {

                custom::BeginGroup();
                {

                    custom::Child("GENERAL", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 2, (GetContentRegionMax().y - (60 + spacing.y * 2) * 2) + 10), true);
                    {

                        static bool popup = false;
                        if (custom::CheckboxClicked("Checkbox true", &checkbox_on)) popup = !popup;

                        if (popup) {
                            ImGui::Begin("Popupbox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
                            {
                                custom::Child("Popupbox", ImVec2(300, 170), ImGuiWindowFlags_NoBringToFrontOnFocus);
                                {
                                    custom::ColorEdit4("Color Palette", col, picker_flags);

                                    custom::Separator_line();

                                    custom::Combo("Combobox", &select1, items, IM_ARRAYSIZE(items), 3);
                                }
                                custom::EndChild();
                            }
                            ImGui::End();
                        }

                        custom::Separator_line();

                        custom::Checkbox("Checkbox false", &checkbox_off);

                        custom::Separator_line();

                        custom::MultiCombo("Multi Combo", multi_num, multi_items, 5);

                        custom::Separator_line();

                        custom::SliderInt("Slider Integer", &slider_int, 1, 100, "%d%%");

                        custom::Separator_line();

                        custom::SliderFloat("Slider Float", &slider_float, 0.f, 5.f, "%.3f [FL]");

                        custom::Separator_line();

                        //ImGui::InputTextWithHint("Text Field", "Hello Mate :/", field, 45, NULL);

                        SetCursorPosY(2000);

                    }
                    custom::EndChild();
                }
                custom::EndGroup();

                ImGui::SameLine();

                custom::BeginGroup();
                {
                    custom::Child("SETTINGS", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 2, ((GetContentRegionMax().y - (60 + spacing.y * 2) * 2) + 10) / 2 - 10), true);
                    {
                        //ImGui::ColorEdit4("Color Palette", col, picker_flags);

                        custom::Separator_line();

                        custom::Combo("Combobox", &select1, items, IM_ARRAYSIZE(items), 3);

                    }
                    custom::EndChild();

                    custom::Child("OTHER", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 2, ((GetContentRegionMax().y - (60 + spacing.y * 2) * 2) + 10) / 2 - 10), true);
                    {

                       // custom::KnobFloat("Dot Slider", &knob, -1.f, 1.f, "%.1f [F]");

                        custom::Separator_line();

                       // custom::Keybind("Keybind", &key, &m);
                       custom::Switch("Switch", &switch_state);

                    }
                    custom::EndChild();

                }
                custom::EndGroup();

            }
            else if (active_tab == 1)
            {
                static bool checkboxes[10];

                custom::BeginGroup();
                {

                    custom::Child("ESP", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 2, (GetContentRegionMax().y - (60 + spacing.y * 2) * 2) + 10), true);
                    {

                        custom::Checkbox("Enable ESP", &checkboxes[0]);

                        custom::Separator_line();

                        custom::Checkbox("Through Walls", &checkboxes[1]);

                        custom::Separator_line();

                        custom::Combo("Bullet Tracer", &bullet_select, bullet, IM_ARRAYSIZE(bullet), 2);

                        custom::Separator_line();

                        custom::Checkbox("Dynamic Boxes", &checkboxes[2]);

                        custom::Separator_line();

                        custom::Checkbox("Dormant", &checkboxes[3]);

                        custom::Separator_line();

                        custom::Checkbox("Shared ESP", &checkboxes[4]);

                        custom::Separator_line();

                        custom::Combo("Sound ESP", &sound_select, sound, IM_ARRAYSIZE(sound), 2);


                    }
                    custom::EndChild();
                }
                custom::EndGroup();

                ImGui::SameLine();

                custom::BeginGroup();
                {
                    custom::Child("CHAMS", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 2, (GetContentRegionMax().y - (60 + spacing.y * 2) * 2) + 10), true);
                    {

                        custom::Checkbox("Enable Chams", &checkboxes[5]);

                        custom::Separator_line();

                        custom::Checkbox("Through Walls", &checkboxes[6]);

                        custom::Separator_line();

                        custom::Combo("Style", &style_select, stylee, IM_ARRAYSIZE(stylee), 2);

                        custom::Separator_line();

                        custom::Checkbox("Backtrack", &checkboxes[7]);

                        custom::Separator_line();

                        custom::Combo("Style ", &style2_select, stylee2, IM_ARRAYSIZE(stylee2), 2);

                        custom::Separator_line();

                        custom::Checkbox("One Shot", &checkboxes[8]);

                        custom::Separator_line();

                        custom::Checkbox("Ragdolls", &checkboxes[9]);

                    }
                    custom::EndChild();
                }
                custom::EndGroup();
            }

            ImGui::PopStyleVar();

            SetCursorPos(ImVec2(spacing.x, region.y - (60 + spacing.y)));
            custom::BeginGroup();
            {

                custom::Child("Page One", ImVec2((GetContentRegionMax().x - (spacing.x * 6)) / 4 - 2, 60), false);
                {

                    if (custom::ThemeButton("0", dark, ImVec2((GetContentRegionMax().x - spacing.x * 2) / 2, GetContentRegionMax().y - spacing.y)))
                    {
                        dark = !dark;
                        ImGui::Notification({ ImGuiToastType_Success, 4000, dark ? "It's black mode!" : "This is the white mode!" });
                    }
                    SameLine();
                    if (custom::Button("b", ImVec2((GetContentRegionMax().x - spacing.x * 2) / 2, GetContentRegionMax().y - spacing.y)));

                }
                custom::EndChild();

                SameLine();

                custom::SeparatorEx(ImGuiSeparatorFlags_Vertical, 2.f);

                SameLine();

                custom::Child("Page Two", ImVec2((GetContentRegionMax().x - (spacing.x * 6)) / 2, 60), false);
                {

                    if (custom::Page(0 == page, "c", ImVec2((GetContentRegionMax().x - spacing.x * 4) / 4, GetContentRegionMax().y - spacing.y))) page = 0;
                    SameLine();
                    if (custom::Page(1 == page, "d", ImVec2((GetContentRegionMax().x - spacing.x * 4) / 4, GetContentRegionMax().y - spacing.y))) page = 1;
                    SameLine();
                    if (custom::Page(2 == page, "e", ImVec2((GetContentRegionMax().x - spacing.x * 4) / 4, GetContentRegionMax().y - spacing.y))) page = 2;
                    SameLine();
                    if (custom::Page(3 == page, "f", ImVec2((GetContentRegionMax().x - spacing.x * 4) / 4, GetContentRegionMax().y - spacing.y))) page = 3;

                }
                custom::EndChild();

                SameLine();

                custom::SeparatorEx(ImGuiSeparatorFlags_Vertical, 2.f);

                SameLine();

                custom::Child("Page Three", ImVec2((GetContentRegionMax().x - (spacing.x * 6)) / 4 - 2, 60), false);
                {
                    if (custom::Button("g", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 3, GetContentRegionMax().y - spacing.y)));
                    SameLine();
                    if (custom::Button("h", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 3, GetContentRegionMax().y - spacing.y)));
                    SameLine();
                    if (custom::Button("e", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 3, GetContentRegionMax().y - spacing.y))) {
                    exit(0);
                    };
                    
                }
                custom::EndChild();

            }
            custom::EndGroup();

        }
        End();

        ImGui::RenderNotifications();
}








int main()
{
    // 初始化GLFW
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui Example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // 启用垂直同步

    // 设置ImGui上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 
// 字体初始化
    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;

    font::inter_semibold_ = io.Fonts->AddFontFromMemoryTTF(inter_semibold, sizeof(inter_semibold), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::icomoon_page_ = io.Fonts->AddFontFromMemoryTTF(icomoon_page, sizeof(icomoon_page), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::icomoon_logo_ = io.Fonts->AddFontFromMemoryTTF(icomoon_page, sizeof(icomoon_page), 30.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::icon_notify_ = io.Fonts->AddFontFromMemoryTTF(icon_notify, sizeof(icon_notify), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

preview_slow_ = LoadTextureFromMemory((void*)preview_slow, sizeof(preview_slow));
    // 设置ImGui风格
    ImGui::StyleColorsDark();

    // 设置平台/渲染器后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // 开始新帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 创建ImGui窗口
        // ImGui::Begin("Hello, ImGui!");
        // ImGui::Text("这是一个ImGui窗口");

        // ImGui::End();

        ShowCustomWidgetsDemo();

        // 渲染
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 清理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}