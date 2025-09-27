#pragma once
#include "GUI/Panels/IPanel.h"
#include "GUI/Data/LightData.h"

class LightEditorPanel : public IPanel {
public:
    using DataType = LightData;
    LightEditorPanel(LightData& data);
    void OnImGuiRender() override;

private:
    LightData& m_Data;
};