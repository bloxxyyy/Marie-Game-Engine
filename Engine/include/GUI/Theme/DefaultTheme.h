#pragma once
#include "imgui.h"
#include <imgui_internal.h>

namespace DefaultTheme
{
    // Colors
    inline ImVec4 Accent = ImVec4(0.20f, 0.50f, 0.90f, 1.0f);

    // Window / gradient colors
    inline ImVec4 GradientTop = ImVec4(0.10f, 0.15f, 0.25f, 1.0f); // dark bluish top
    inline ImVec4 GradientBottom = ImVec4(0.05f, 0.08f, 0.18f, 1.0f); // darker bluish bottom

    // Fonts
    inline ImFont* RegularFont = nullptr;
    inline ImFont* TopBarFont = nullptr;
    inline ImFont* HeaderFont = nullptr;

    // Apply style
    void Apply();

    // Helpers
    inline void PushTitleFont()
    {
        if (TopBarFont) ImGui::PushFont(TopBarFont);
    }
    inline void PopTitleFont()
    {
        if (TopBarFont) ImGui::PopFont();
    }

    inline void DrawWindowGradient()
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (!window) return;

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 pos = window->Pos;
        ImVec2 size = window->Size;

        drawList->AddRectFilledMultiColor(
            pos,
            ImVec2(pos.x + size.x, pos.y + size.y),
            ImColor(GradientTop),
            ImColor(GradientTop),
            ImColor(GradientBottom),
            ImColor(GradientBottom)
        );
    }

    inline void BeginPanel(const char* name, bool* open = nullptr)
    {
        PushTitleFont();
        ImGui::Begin(name, open);
        PopTitleFont();

        // Automatic gradient
        DrawWindowGradient();
    }
}
