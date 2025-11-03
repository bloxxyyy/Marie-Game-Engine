#include "GUI/LightEditor/LightEditorPanel.h"
#include "imgui.h"
#include <functional>

LightEditorPanel::LightEditorPanel(LightData& data, std::function<void()> onDirty) : m_Data(data), m_OnDirty(onDirty) {}

void LightEditorPanel::OnImGuiRender()
{
    if (!IsOpen) return;

    ImGui::Begin("Light Editor", &IsOpen);
    ImGui::PushItemWidth(-1.0f);

    // --- Position ---
    ImGui::Text("Position");
    if (ImGui::DragFloat3("##Position", &m_Data.position[0], 0.1f)) {
        if (m_OnDirty) m_OnDirty();
    }
    ImGui::Spacing();

    // --- Color ---
    ImGui::Text("Color");
    if (ImGui::ColorEdit3("##Color", &m_Data.color[0])) {
        if (m_OnDirty) m_OnDirty();
    }
    ImGui::Spacing();

    ImGui::Separator();
    ImGui::Spacing();

    // --- Lighting Properties ---
    ImGui::Text("Ambient Strength");
    if (ImGui::SliderFloat("##Ambient", &m_Data.ambientStrength, 0.0f, 1.0f)) {
        if (m_OnDirty) m_OnDirty();
    }
    ImGui::Spacing();

    ImGui::Text("Diffuse Strength");
    if (ImGui::SliderFloat("##Diffuse", &m_Data.diffuseStrength, 0.0f, 5.0f)) {
        if (m_OnDirty) m_OnDirty();
    }
    ImGui::Spacing();

    ImGui::Text("Specular Strength");
    if (ImGui::SliderFloat("##Specular", &m_Data.specularStrength, 0.0f, 5.0f)) {
        if (m_OnDirty) m_OnDirty();
    }
    ImGui::Spacing();

    ImGui::Text("Shininess");
    if (ImGui::SliderFloat("##Shininess", &m_Data.shininess, 2.0f, 256.0f)) {
        if (m_OnDirty) m_OnDirty();
    }

    ImGui::PopItemWidth();
    ImGui::End();
}