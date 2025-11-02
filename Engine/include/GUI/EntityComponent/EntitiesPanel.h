#pragma once
#include "GUI/IPanelView.h"
#include "GUI/Data/EntityListData.h"

class EntitiesPanel : public IPanelView {
public:

    // This panel declares its dependency on EntityListData.
    using DataType = EntityListData;

    EntitiesPanel(DataType& data);
    void OnImGuiRender() override;

private:
    DataType& m_Data;
};