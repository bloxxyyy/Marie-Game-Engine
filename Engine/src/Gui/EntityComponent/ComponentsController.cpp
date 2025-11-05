#include "ECS/ECSRegistry.h"
#include "GUI/Data/EntityListData.h"
#include <GUI/EntityComponent/ComponentsController.h>
#include "GUI/EntityComponent/ComponentsPanel.h"

ComponentsPanelController::ComponentsPanelController(ECSRegistry& registry, EntityListData& entityListData) : m_Registry(registry), m_EntityListData(entityListData)
{
    m_View = std::make_unique<ComponentsPanel>(m_ViewData, [this]() {
        this->m_IsDirty = true;
    });
}

void ComponentsPanelController::UpdatePush() {
    m_IsDirty = false;
    m_ViewData.selectedEntity = m_EntityListData.selectedEntity;

    // Clear old data
    m_ViewData.tag.reset();
    m_ViewData.transform.reset();
    m_ViewData.render.reset();
    m_ViewData.light.reset(); // THE CHANGE: Clear light data too

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
        // THE CHANGE: Also check for and push LightComponent data.
        if (m_Registry.HasComponent<LightComponent>(m_ViewData.selectedEntity)) {
            m_ViewData.light = m_Registry.GetComponent<LightComponent>(m_ViewData.selectedEntity);
        }
    }
}

void ComponentsPanelController::UpdatePull() {
    if (!m_IsDirty) {
        return;
    }

    if (m_ViewData.selectedEntity != NULL_ENTITY) {
        if (m_ViewData.tag.has_value()) {
            m_Registry.GetComponent<TagComponent>(m_ViewData.selectedEntity) = *m_ViewData.tag;
        }
        if (m_ViewData.transform.has_value()) {
            m_Registry.GetComponent<TransformComponent>(m_ViewData.selectedEntity) = *m_ViewData.transform;
        }
        if (m_ViewData.light.has_value()) {
            m_Registry.GetComponent<LightComponent>(m_ViewData.selectedEntity) = *m_ViewData.light;
        }
    }
}

IPanelView& ComponentsPanelController::GetView() {
    return *m_View;
}