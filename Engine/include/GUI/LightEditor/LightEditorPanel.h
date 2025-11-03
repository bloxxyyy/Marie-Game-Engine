#pragma once
#include "GUI/IPanelView.h"
#include "GUI/Data/LightData.h"
#include <functional>

class LightEditorPanel : public IPanelView {
public:
    using DataType = LightData;
    LightEditorPanel(DataType& data, std::function<void()> onDirty);
    void OnImGuiRender() override;

private:
    LightData& m_Data;
    std::function<void()> m_OnDirty;
};