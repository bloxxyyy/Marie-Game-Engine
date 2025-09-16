#include "GUI/Panels/MonitoringPanel.h"
#include "imgui.h"
#include <GUI/Theme/DefaultTheme.h>

MonitoringPanel::MonitoringPanel(EngineStats& s) : stats(s) {}

void MonitoringPanel::OnImGuiRender()
{
    if (!IsOpen) return;

    ImGui::SetNextWindowSizeConstraints(ImVec2(320.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
    DefaultTheme::BeginPanel("Monitoring Window", &IsOpen);
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    auto HeaderTitle = [drawList](const char* text)
        {
            if (DefaultTheme::HeaderFont) ImGui::PushFont(DefaultTheme::HeaderFont);

            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImGui::Text("%s", text);

            ImVec2 textSize = ImGui::CalcTextSize(text);
            float panelWidth = ImGui::GetWindowSize().x;
            float lineWidth = panelWidth * 0.8f;

            float lineOffsetY = 6.0f; // tiny spacing between header text and line
            drawList->AddLine(
                ImVec2(pos.x, pos.y + textSize.y + lineOffsetY),
                ImVec2(pos.x + lineWidth, pos.y + textSize.y + lineOffsetY),
                IM_COL32(200, 200, 200, 255),
                1.5f
            );

            if (DefaultTheme::HeaderFont) ImGui::PopFont();

            // Add margin after header
            ImGui::Dummy(ImVec2(0.0f, 4.0f)); // spacing after line
        };

    auto DrawValue = [](const char* title, double value, const ImVec4& color, const char* fmt = "%.1f")
        {
            float labelWidth = 120.0f; // fixed width for all titles
            ImGui::TextUnformatted(title);
            ImGui::SameLine(labelWidth);
            ImGui::TextColored(color, fmt, value);
        };

    // ----- FPS Section -----
    HeaderTitle("FPS");

    ImVec4 fpsColor = stats.fps < 15.0f ? ImVec4(1, 0, 0, 1) : (stats.fps < 30.0f ? ImVec4(1, 0.5f, 0, 1) : ImVec4(0, 1, 0, 1));
    DrawValue("Instant FPS", stats.fps, fpsColor);

    ImVec4 avgFpsColor = stats.avgFPS < 15.0f ? ImVec4(1, 0, 0, 1) : (stats.avgFPS < 30.0f ? ImVec4(1, 0.5f, 0, 1) : ImVec4(0, 1, 0, 1));
    DrawValue("Avg FPS", stats.avgFPS, avgFpsColor);

    ImVec4 deltaColor = ((stats.deltaTime * 1000.0f) > 66.6f) ? ImVec4(1, 0, 0, 1) : (((stats.deltaTime * 1000.0f) > 33.3f) ? ImVec4(1, 0.5f, 0, 1) : ImVec4(0, 1, 0, 1));
    DrawValue("Frame Time", stats.deltaTime * 1000.0f, deltaColor, "%.2f ms");

    // Margin before next section
    ImGui::Dummy(ImVec2(0.0f, 8.0f));

    // ----- CPU Section -----
    HeaderTitle("CPU");

    ImVec4 cpuColor = stats.cpuUsage > 80.0f ? ImVec4(1, 0, 0, 1) : (stats.cpuUsage > 50.0f ? ImVec4(1, 0.5f, 0, 1) : ImVec4(0, 1, 0, 1));
    DrawValue("CPU Usage", stats.cpuUsage, cpuColor, "%.1f%%");

    ImGui::End();
}
