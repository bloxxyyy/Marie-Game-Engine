#include "imgui.h"
#include "imgui_stdlib.h"
#include <GUI/EntityComponent/ComponentsPanel.h>

ComponentsPanel::ComponentsPanel(DataType& data, std::function<void()> onDirty)
    : m_Data(data), m_OnDirty(onDirty) {
}

void ComponentsPanel::OnImGuiRender()
{
    if (!IsOpen) return;
    ImGui::Begin("Component Inspector", &IsOpen);

    if (m_Data.selectedEntity != NULL_ENTITY)
    {
        if (m_Data.tag.has_value()) {
            ImGui::Text("Tag Component");
            if (ImGui::InputText("Tag", &m_Data.tag->tag)) {
                if (m_OnDirty) m_OnDirty();
            }
            ImGui::Separator();
        }

        if (m_Data.transform.has_value()) {
            ImGui::Text("Transform Component");
            bool changed = false;
            changed |= ImGui::DragFloat3("Position", &m_Data.transform->position[0], 0.1f);
            changed |= ImGui::DragFloat3("Rotation", &m_Data.transform->rotation[0], 0.1f);
            changed |= ImGui::DragFloat3("Scale", &m_Data.transform->scale[0], 0.1f);
            if (changed && m_OnDirty) m_OnDirty();
            ImGui::Separator();
        }

        // THE CHANGE: Add the rendering logic for the LightComponent.
        if (m_Data.light.has_value()) {
            ImGui::Text("Light Component");
            bool changed = false;
            changed |= ImGui::ColorEdit3("Color", &m_Data.light->color[0]);
            changed |= ImGui::SliderFloat("Ambient", &m_Data.light->ambientStrength, 0.0f, 1.0f);
            changed |= ImGui::SliderFloat("Diffuse", &m_Data.light->diffuseStrength, 0.0f, 5.0f);
            changed |= ImGui::SliderFloat("Specular", &m_Data.light->specularStrength, 0.0f, 5.0f);
            changed |= ImGui::SliderFloat("Shininess", &m_Data.light->shininess, 2.0f, 256.0f);
            if (changed && m_OnDirty) m_OnDirty();
            ImGui::Separator();
        }

        if (m_Data.render.has_value()) {
            ImGui::Text("Render Component");
            ImGui::Text("Has a Mesh and Material.");
            ImGui::Separator();
        }
    }
    else
    {
        ImGui::Text("No entity selected.");
    }

    ImGui::End();
}