#include "GUI/Monitoring/MonitoringController.h"
#include "GUI/Monitoring/MonitoringPanel.h"

MonitoringController::MonitoringController(EngineStats& targetStats)
    : m_TargetStats(targetStats) {
    m_View = std::make_unique<MonitoringPanel>(m_ViewData);
}

void MonitoringController::UpdatePush() {
    // 1. Instantly update the text values so the readout feels highly responsive
    m_ViewData.deltaTime = m_TargetStats.deltaTime;
    m_ViewData.fps = m_TargetStats.fps;
    m_ViewData.avgFPS = m_TargetStats.avgFPS;
    m_ViewData.cpuUsage = m_TargetStats.cpuUsage;

    // 2. Accumulate up to 1200 frames in a background buffer
    static std::vector<double> historyBuffer;
    historyBuffer.push_back(m_TargetStats.fps);
    
    // Keep it capped at your max history limit
    if (historyBuffer.size() > m_ViewData.maxHistory) {
        historyBuffer.erase(historyBuffer.begin());
    }

    // 3. Once a second has passed, push the snapshot to the View
    static double timer = 0.0;
    timer += m_TargetStats.deltaTime;

    if (timer >= 1.0) {
        m_ViewData.fpsHistory = historyBuffer;
        timer -= 1.0; 
    }
}

void MonitoringController::UpdatePull() {
    // Read-only panel, nothing to pull
}

IPanelView& MonitoringController::GetView() {
    return *m_View;
}