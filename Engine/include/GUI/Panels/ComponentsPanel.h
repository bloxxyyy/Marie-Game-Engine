#pragma once
#include "GUI/Panels/IPanelView.h"
#include "GUI/Data/ComponentInspectorData.h"

class ComponentsPanel : public IPanelView {
public:
    using DataType = ComponentInspectorData;

    ComponentsPanel(DataType& data);
    void OnImGuiRender() override;

private:
    DataType& m_Data;
};