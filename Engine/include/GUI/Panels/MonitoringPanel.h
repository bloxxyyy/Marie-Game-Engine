#pragma once
#include "GUI/Panels/IPanelView.h"
#include "GUI/Data/EngineStats.h"

class MonitoringPanel : public IPanelView {
public:
    using DataType = EngineStats;
    MonitoringPanel(EngineStats& data);
    void OnImGuiRender() override;

private:
    EngineStats& m_Data;
};