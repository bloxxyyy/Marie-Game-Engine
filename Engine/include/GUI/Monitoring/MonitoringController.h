#pragma once
#include "GUI/IPanelController.h"
#include "GUI/Data/EngineStats.h"
#include <memory>

class MonitoringPanel;

class MonitoringController : public IPanelController {
public:
    MonitoringController(EngineStats& targetStats);

    void UpdatePush() override;
    void UpdatePull() override;
    IPanelView& GetView() override;

private:
    EngineStats& m_TargetStats; // backend data
    EngineStats m_ViewData;     // snapshot for view
    std::unique_ptr<MonitoringPanel> m_View;
};