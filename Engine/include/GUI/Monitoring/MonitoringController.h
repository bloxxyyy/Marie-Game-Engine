#pragma once
#include "GUI/IPanelController.h"
#include "GUI/Data/EngineStats.h"
#include <memory>

class GuiManager;
class MonitoringPanel;
struct EngineStats;

class MonitoringController : public IPanelController {
public:
    MonitoringController(EngineStats& targetStats);

    void UpdatePush() override;
    void UpdatePull() override;
    IPanelView& GetView() override;

private:
    EngineStats& m_TargetStats; // Reference to the Model
    EngineStats m_ViewData;     // The ViewModel
    std::unique_ptr<MonitoringPanel> m_View;
};