#include "GUI/EntityComponent/EntitiesController.h"
#include "GUI/EntityComponent/EntitiesPanel.h"
#include "ECS/ECSRegistry.h"
#include "ECS/Components.h"

EntitiesPanelController::EntitiesPanelController(ECSRegistry& registry)  : m_Registry(registry)
{
    m_View = std::make_unique<EntitiesPanel>(m_ViewData);
}

void EntitiesPanelController::UpdatePush() {
    // Preserve the selection from the previous frame.
    Entity selected = m_ViewData.selectedEntity;
    m_ViewData.entities.clear();

    // Re-populate the list from the registry.
    auto& tagMap = m_Registry.GetComponentMap<TagComponent>();
    for (auto const& [entity, tagComp] : tagMap) {
        m_ViewData.entities.push_back({ entity, tagComp.tag });
    }
    m_ViewData.selectedEntity = selected;
}

void EntitiesPanelController::UpdatePull() {
    // PULL: The selection state is managed internally by this controller
}

IPanelView& EntitiesPanelController::GetView() {
    return *m_View;
}