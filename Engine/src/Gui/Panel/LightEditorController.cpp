#include "GUI/Panels/LightEditorController.h"
#include "GUI/Panels/LightEditorPanel.h"
#include "Light.h"

LightEditorController::LightEditorController(Light& targetLight)  : m_TargetLight(targetLight)
{
    m_View = std::make_unique<LightEditorPanel>(m_ViewData);
}

void LightEditorController::UpdatePush() {
    m_ViewData.position                 = m_TargetLight.position;
    m_ViewData.ambientStrength  = m_TargetLight.ambientStrength;
    m_ViewData.color                       = m_TargetLight.color;
    m_ViewData.diffuseStrength    = m_TargetLight.diffuseStrength;
    m_ViewData.shininess               = m_TargetLight.shininess;
    m_ViewData.specularStrength = m_TargetLight.specularStrength;
}

void LightEditorController::UpdatePull() {
    m_TargetLight.position                 = m_ViewData.position;
    m_TargetLight.ambientStrength  = m_ViewData.ambientStrength;
    m_TargetLight.color                       = m_ViewData.color;
    m_TargetLight.diffuseStrength    = m_ViewData.diffuseStrength;
    m_TargetLight.shininess               = m_ViewData.shininess;
    m_TargetLight.specularStrength = m_ViewData.specularStrength;
}

IPanelView& LightEditorController::GetView() {
    return *m_View;
}