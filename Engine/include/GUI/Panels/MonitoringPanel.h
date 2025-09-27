#pragma once
#include "GUI/Panels/IPanel.h"
#include "GUI/Data/EngineStats.h"

class MonitoringPanel : public IPanel {
public:
    using DataType = EngineStats;
    MonitoringPanel(EngineStats& data);
    void OnImGuiRender() override;

private:
    EngineStats& m_Data;
};