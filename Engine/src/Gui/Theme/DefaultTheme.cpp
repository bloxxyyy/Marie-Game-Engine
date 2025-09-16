#include "GUI/Theme/DefaultTheme.h"

void DefaultTheme::Apply()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Layout
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    style.FrameBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;

    style.ItemSpacing = ImVec2(8, 6);
    style.ItemInnerSpacing = ImVec2(6, 4);

    style.FramePadding.y = 14.0f; // topbar height
    style.WindowPadding.y = 10.0f;

    // Colors
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = GradientBottom;
    colors[ImGuiCol_TitleBg] = GradientTop;
    colors[ImGuiCol_TitleBgActive] = GradientTop;
    colors[ImGuiCol_TitleBgCollapsed] = GradientTop;

    colors[ImGuiCol_Header] = ImVec4(0.18f, 0.28f, 0.25f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.36f, 0.32f, 1.0f);
    colors[ImGuiCol_HeaderActive] = Accent;

    colors[ImGuiCol_Button] = ImVec4(0.18f, 0.28f, 0.25f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.22f, 0.36f, 0.32f, 1.0f);
    colors[ImGuiCol_ButtonActive] = Accent;

    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.18f, 0.22f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.22f, 0.27f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = Accent;

    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.18f, 0.22f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.32f, 0.40f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.25f, 0.32f, 1.0f);
}
