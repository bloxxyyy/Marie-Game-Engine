#pragma once
#include "GUI/IPanelView.h"
#include "GUI/Data/ComponentInspectorData.h"
#include <functional>

class ComponentsPanel : public IPanelView {
public:
    using DataType = ComponentInspectorData;

    ComponentsPanel(DataType& data, std::function<void()> onDirty);
    void OnImGuiRender() override;

private:
    DataType& m_Data;
    std::function<void()> m_OnDirty;
};