#include "GUI/Panels/MonitoringPanel.h"
#include "imgui.h"

MonitoringPanel::MonitoringPanel(EngineStats& s) : stats(s) {}

void MonitoringPanel::OnImGuiRender() {

    if (!IsOpen) return;

    ImGui::Begin("Monitoring Window", &IsOpen);

    // FPS + frame timing
    ImGui::Text("Instant FPS: %.1f", stats.fps);
    ImGui::Text("Avg FPS: %.1f", stats.avgFPS);
    ImGui::Text("Frame time: %.3f ms", stats.deltaTime * 1000.0f);

    ImGui::Separator();

    // CPU/GPU usage
    ImGui::Text("CPU Usage: %.1f%%", stats.cpuUsage);

    ImGui::End();
}
