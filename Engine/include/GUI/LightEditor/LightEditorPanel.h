#pragma once
#include "GUI/IPanelView.h"
#include "GUI/Data/LightData.h"

class LightEditorPanel : public IPanelView {
public:
    using DataType = LightData;
    LightEditorPanel(LightData& data);
    void OnImGuiRender() override;

private:
    LightData& m_Data;
};