#include "GUI/LightEditor/LightEditorController.h"
#include "GUI/LightEditor/LightEditorPanel.h"
#include <ECS/ECSRegistry.h>
#include <ECS/Components.h>
#include "GUI/Data/EntityListData.h"


LightEditorController::LightEditorController(ECSRegistry& registry, EntityListData& entityListData) : m_Registry(registry), m_EntityListData(entityListData)
{
    m_View = std::make_unique<LightEditorPanel>(m_ViewData, [this]() {
        this->m_IsDirty = true;
    });
}

void LightEditorController::UpdatePush() {
    m_IsDirty = false;
    Entity selectedEntity = m_EntityListData.selectedEntity;

    if (selectedEntity != NULL_ENTITY && m_Registry.HasComponent<LightComponent>(selectedEntity)) {
        auto& lightComp = m_Registry.GetComponent<LightComponent>(selectedEntity);
        m_ViewData.color = lightComp.color;
        m_ViewData.ambientStrength = lightComp.ambientStrength;
        m_ViewData.diffuseStrength = lightComp.diffuseStrength;
        m_ViewData.shininess = lightComp.shininess;
        m_ViewData.specularStrength = lightComp.specularStrength;

        if (m_Registry.HasComponent<TransformComponent>(selectedEntity)) {
            m_ViewData.position = m_Registry.GetComponent<TransformComponent>(selectedEntity).position;
        }
    }
    else {
        m_ViewData = LightData{}; // Reset to default
    }
}

void LightEditorController::UpdatePull() {

    if (!m_IsDirty) {
        return;
    }

    Entity selectedEntity = m_EntityListData.selectedEntity;

    if (selectedEntity != NULL_ENTITY && m_Registry.HasComponent<LightComponent>(selectedEntity)) {
        auto& lightComp = m_Registry.GetComponent<LightComponent>(selectedEntity);
        lightComp.color = m_ViewData.color;
        lightComp.ambientStrength = m_ViewData.ambientStrength;
        lightComp.color = m_ViewData.color;
        lightComp.diffuseStrength = m_ViewData.diffuseStrength;
        lightComp.shininess = m_ViewData.shininess;
        lightComp.specularStrength = m_ViewData.specularStrength;

        if (m_Registry.HasComponent<TransformComponent>(selectedEntity)) {
            m_Registry.GetComponent<TransformComponent>(selectedEntity).position = m_ViewData.position;
        }
    }
}



IPanelView& LightEditorController::GetView() {
    return *m_View;
}