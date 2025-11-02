#include "GUI/EntityComponent/ComponentsController.h"
#include "GUI/EntityComponent/ComponentsPanel.h"
#include "ECS/ECSRegistry.h"
#include "GUI/Data/EntityListData.h"

ComponentsPanelController::ComponentsPanelController(ECSRegistry& registry, EntityListData& entityListData) : m_Registry(registry), m_EntityListData(entityListData)
{
    m_View = std::make_unique<ComponentsPanel>(m_ViewData);
}

void ComponentsPanelController::UpdatePush() {
    // PUSH: Get the selected entity and populate our view model.
    m_ViewData.selectedEntity = m_EntityListData.selectedEntity;

    // Clear old optional data
    m_ViewData.tag.reset();
    m_ViewData.transform.reset();
    m_ViewData.render.reset();

    if (m_ViewData.selectedEntity != NULL_ENTITY) {
        if (m_Registry.HasComponent<TagComponent>(m_ViewData.selectedEntity)) {
            m_ViewData.tag = m_Registry.GetComponent<TagComponent>(m_ViewData.selectedEntity);
        }
        if (m_Registry.HasComponent<TransformComponent>(m_ViewData.selectedEntity)) {
            m_ViewData.transform = m_Registry.GetComponent<TransformComponent>(m_ViewData.selectedEntity);
        }
        if (m_Registry.HasComponent<RenderComponent>(m_ViewData.selectedEntity)) {
            m_ViewData.render = m_Registry.GetComponent<RenderComponent>(m_ViewData.selectedEntity);
        }
    }
}

void ComponentsPanelController::UpdatePull() {
    // If a value was edited in the UI, write it back to the ECS.
    if (m_ViewData.selectedEntity != NULL_ENTITY) {
        if (m_ViewData.tag.has_value()) {
            m_Registry.GetComponent<TagComponent>(m_ViewData.selectedEntity) = *m_ViewData.tag;
        }
        if (m_ViewData.transform.has_value()) {
            m_Registry.GetComponent<TransformComponent>(m_ViewData.selectedEntity) = *m_ViewData.transform;
        }
    }
}

IPanelView& ComponentsPanelController::GetView() {
    return *m_View;
}