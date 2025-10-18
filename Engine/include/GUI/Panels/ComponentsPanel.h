#pragma once
#include "GUI/Panels/IPanel.h"
#include "GUI/Data/ComponentInspectorData.h"

class ComponentsPanel : public IPanel {
public:
    using DataType = ComponentInspectorData;

    ComponentsPanel(DataType& data);
    void OnImGuiRender() override;

private:
    DataType& m_Data;
};