#pragma once
#include "GUI/IPanelController.h"
#include "GUI/Data/LightData.h"
#include <memory>

class ECSRegistry;
class LightEditorPanel;
struct EntityListData;

class LightEditorController : public IPanelController {
public:
    LightEditorController(ECSRegistry& registry, EntityListData& entityListData);

    void UpdatePush() override;
    void UpdatePull() override;
    IPanelView& GetView() override;

private:
    ECSRegistry& m_Registry;
    EntityListData& m_EntityListData; // To know which entity is selected
    LightData m_ViewData;
    std::unique_ptr<LightEditorPanel> m_View;
    bool m_IsDirty = false;
};