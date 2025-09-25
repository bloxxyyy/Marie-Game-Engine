#include "GUI/Panels/LightEditorPanel.h"
#include "imgui.h"

LightEditorPanel::LightEditorPanel(LightData& data) : m_Data(data) {}

void LightEditorPanel::OnImGuiRender()
{
    if (!IsOpen) return;
    ImGui::Begin("Light Editor", &IsOpen);

    ImGui::PushItemWidth(-1.0f); // Make all widgets in this window span the full width by default

    ImGui::Text("Position");

    // Use '##' to hide the label but provide a unique ID.
    ImGui::DragFloat3("##Position", &m_Data.position[0], 0.1f);
    ImGui::Spacing();

    ImGui::Text("Color");
    ImGui::ColorEdit3("##Color", &m_Data.color[0]);
    ImGui::Spacing();

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Ambient Strength");
    ImGui::SliderFloat("##Ambient", &m_Data.ambientStrength, 0.0f, 1.0f);
    ImGui::Spacing();

    ImGui::Text("Diffuse Strength");
    ImGui::SliderFloat("##Diffuse", &m_Data.diffuseStrength, 0.0f, 5.0f);
    ImGui::Spacing();

    ImGui::Text("Specular Strength");
    ImGui::SliderFloat("##Specular", &m_Data.specularStrength, 0.0f, 5.0f);
    ImGui::Spacing();

    ImGui::Text("Shininess");
    ImGui::SliderFloat("##Shininess", &m_Data.shininess, 2.0f, 256.0f);

    ImGui::PopItemWidth();
    ImGui::End();
}