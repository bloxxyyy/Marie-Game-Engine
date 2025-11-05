#pragma once
#include "GUI/IPanelController.h"
#include "GUI/Data/ComponentInspectorData.h"
#include <memory>

class ECSRegistry;
class ComponentsPanel;
struct EntityListData;

class ComponentsPanelController : public IPanelController {
public:
    ComponentsPanelController(ECSRegistry& registry, EntityListData& entityListData);

    void UpdatePush() override;
    void UpdatePull() override;
    IPanelView& GetView() override;

private:
    ECSRegistry& m_Registry;
    EntityListData& m_EntityListData;
    ComponentInspectorData m_ViewData;
    std::unique_ptr<ComponentsPanel> m_View;
    bool m_IsDirty = false;
};