#include "GUI/Monitoring/MonitoringPanel.h"
#include <vector>
#include <imgui.h>
#include <implot.h>
#include <GUI/Theme/DefaultTheme.h>

MonitoringPanel::MonitoringPanel(EngineStats& data) : m_Data(data) {}

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

        float lineOffsetY = 6.0f;
        drawList->AddLine(
            ImVec2(pos.x, pos.y + textSize.y + lineOffsetY),
            ImVec2(pos.x + lineWidth, pos.y + textSize.y + lineOffsetY),
            IM_COL32(200, 200, 200, 255),
            1.5f
        );

        if (DefaultTheme::HeaderFont) ImGui::PopFont();
        ImGui::Dummy(ImVec2(0.0f, 4.0f));
    };

    auto DrawValue = [](const char* title, double value, const ImVec4& color, const char* fmt = "%.1f")
    {
        float labelWidth = 120.0f;
        ImGui::TextUnformatted(title);
        ImGui::SameLine(labelWidth);
        ImGui::TextColored(color, fmt, value);
    };

    // ----- FPS Section -----
    HeaderTitle("FPS");

    ImVec4 fpsColor = m_Data.fps < 15.0f ? ImVec4(1, 0, 0, 1) : (m_Data.fps < 30.0f ? ImVec4(1, 0.5f, 0, 1) : ImVec4(0, 1, 0, 1));
    DrawValue("Instant FPS", m_Data.fps, fpsColor);

    ImVec4 avgFpsColor = m_Data.avgFPS < 15.0f ? ImVec4(1, 0, 0, 1) : (m_Data.avgFPS < 30.0f ? ImVec4(1, 0.5f, 0, 1) : ImVec4(0, 1, 0, 1));
    DrawValue("Avg FPS", m_Data.avgFPS, avgFpsColor);

    ImVec4 deltaColor = ((m_Data.deltaTime * 1000.0f) > 66.6f) ? ImVec4(1, 0, 0, 1) : (((m_Data.deltaTime * 1000.0f) > 33.3f) ? ImVec4(1, 0.5f, 0, 1) : ImVec4(0, 1, 0, 1));
    DrawValue("Frame Time", m_Data.deltaTime * 1000.0f, deltaColor, "%.2f ms");

    // ----- FPS History Graph (ImPlot) -----
    ImGui::Dummy(ImVec2(0.0f, 8.0f));
    HeaderTitle("FPS History");

    if (!m_Data.fpsHistory.empty()) {
        
        // Use static so it remembers the absolute lowest value across frames
        static double min_fps = 99999.0; 
        
        double max_fps = -1.0;
        double current_fps = m_Data.fpsHistory.back(); // The most recent frame in the history

        // Calculate Max for this specific batch, and update the persistent Min
        for (double f : m_Data.fpsHistory) {
            if (f > max_fps) max_fps = f;
            
            // Only update min_fps if we find a new absolute lowest value
            // (f > 0.0 prevents an empty/starting buffer from locking min_fps at 0)
            if (f < min_fps && f > 0.0) {
                min_fps = f;
            }
        }
        
        if (min_fps == 99999.0) min_fps = 0.0;
        if (max_fps < 60.0) max_fps = 60.0; // Keep a minimum ceiling

        if (ImPlot::BeginPlot("##FPSHistory", ImVec2(-1, 150), ImPlotFlags_NoTitle | ImPlotFlags_NoInputs | ImPlotFlags_NoMenus | ImPlotFlags_NoLegend)) {
            
            // X Axis: No decorations. 
            // Y Axis: No Tick Labels (We disable default numbers so our custom Tags look clean)
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoTickLabels);
            
            ImPlot::SetupAxisLimits(ImAxis_X1, 0, m_Data.fpsHistory.size(), ImPlotCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, max_fps * 1.1, ImPlotCond_Always); 
            
            // Shaded Area
            ImPlot::PlotShaded("FPS", m_Data.fpsHistory.data(), static_cast<int>(m_Data.fpsHistory.size()), -INFINITY, 1.0, 0.0, {
                ImPlotProp_FillColor, ImVec4(0.2f, 0.8f, 0.2f, 1.0f),
                ImPlotProp_FillAlpha, 0.2f
            });
            
            // Solid Line
            ImPlot::PlotLine("FPS", m_Data.fpsHistory.data(), static_cast<int>(m_Data.fpsHistory.size()), 1.0, 0.0, {
                ImPlotProp_LineColor, ImVec4(0.2f, 0.8f, 0.2f, 1.0f),
                ImPlotProp_LineWeight, 1.5f
            });
            
            ImPlot::Annotation(0.0, max_fps,     ImVec4(0.8f, 0.2f, 0.2f, 1.0f), ImVec2(5, 0), true, "Max: %.0f", max_fps);
            ImPlot::Annotation(0.0, min_fps,     ImVec4(0.2f, 0.6f, 0.8f, 1.0f), ImVec2(5, 0), true, "Min: %.0f", min_fps);
            
            double right_edge = static_cast<double>(m_Data.fpsHistory.size());
            ImPlot::Annotation(right_edge, current_fps, ImVec4(0.2f, 0.8f, 0.2f, 1.0f), ImVec2(-5, 0), true, "Cur: %.0f", current_fps);
            ImPlot::EndPlot();
        }
    }

    // ----- CPU Section -----
    ImGui::Dummy(ImVec2(0.0f, 8.0f));
    HeaderTitle("CPU");

    ImVec4 cpuColor = m_Data.cpuUsage > 80.0f ? ImVec4(1, 0, 0, 1) : (m_Data.cpuUsage > 50.0f ? ImVec4(1, 0.5f, 0, 1) : ImVec4(0, 1, 0, 1));
    DrawValue("CPU Usage", m_Data.cpuUsage, cpuColor, "%.1f%%");

    ImGui::End();
}