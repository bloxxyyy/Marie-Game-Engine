#pragma once
#include "GUI/IPanelController.h"
#include "GUI/Data/EntityListData.h"
#include <memory>

class ECSRegistry;
class EntitiesPanel;

class EntitiesPanelController : public IPanelController {
public:
    EntitiesPanelController(ECSRegistry& registry);

    void UpdatePush() override;
    void UpdatePull() override;
    IPanelView& GetView() override;

    // Public getter so other controllers can access the selection state.
    EntityListData& GetViewData() { return m_ViewData; }

private:
    ECSRegistry& m_Registry;
    EntityListData m_ViewData;
    std::unique_ptr<EntitiesPanel> m_View;
};