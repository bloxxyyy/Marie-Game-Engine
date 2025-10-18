#pragma once
#include "GUI/Panels/IPanel.h"
#include "GUI/Data/EntityListData.h"

class EntitiesPanel : public IPanel {
public:

    // This panel declares its dependency on EntityListData.
    using DataType = EntityListData;

    EntitiesPanel(DataType& data);
    void OnImGuiRender() override;

private:
    DataType& m_Data;
};