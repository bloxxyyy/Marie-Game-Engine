#include "GUI/Monitoring/MonitoringController.h"
#include "GUI/Monitoring/MonitoringPanel.h"
#include "GUI/Data/EngineStats.h"

MonitoringController::MonitoringController(EngineStats& targetStats)
    : m_TargetStats(targetStats)
{
    m_View = std::make_unique<MonitoringPanel>(m_ViewData);
}

void MonitoringController::UpdatePush() {
    m_ViewData = m_TargetStats;
}

void MonitoringController::UpdatePull() {
    // This is a read-only panel, so we do nothing.
}

IPanelView& MonitoringController::GetView() {
    return *m_View;
}