#pragma once
#include "GUI/IPanelView.h"
#include "GUI/Data/ComponentInspectorData.h"

class ComponentsPanel : public IPanelView {
public:
    using DataType = ComponentInspectorData;

    ComponentsPanel(DataType& data);
    void OnImGuiRender() override;

private:
    DataType& m_Data;
};