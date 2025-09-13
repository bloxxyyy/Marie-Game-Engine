#pragma once
#include "GUI/Panels/IPanel.h"
#include "GUI/Data/EngineStats.h"

class MonitoringPanel : public IPanel {
    EngineStats& stats;
public:
    MonitoringPanel(EngineStats& s);
    void OnImGuiRender() override;
};