#include "GUI/EntityComponent/ComponentsPanel.h"
#include "imgui.h"
#include "imgui_stdlib.h"

ComponentsPanel::ComponentsPanel(DataType& data) : m_Data(data) {}

void ComponentsPanel::OnImGuiRender()
{
    if (!IsOpen) return;
    ImGui::Begin("Component Inspector", &IsOpen);

    if (m_Data.selectedEntity != NULL_ENTITY)
    {
        // For each component, we check if our data struct has a value.
        // We also check the return value of the widget to mark the data as dirty.
        if (m_Data.tag.has_value()) {
            ImGui::Text("Tag Component");
            if (ImGui::InputText("Tag", &m_Data.tag->tag)) {
                // We don't have a dirty flag system for this panel yet,
                // but the PULL logic in the Engine will save it anyway.
            }
            ImGui::Separator();
        }

        if (m_Data.transform.has_value()) {
            ImGui::Text("Transform Component");
            ImGui::DragFloat3("Position", &m_Data.transform->position[0], 0.1f);
            ImGui::DragFloat3("Rotation", &m_Data.transform->rotation[0], 0.1f);
            ImGui::DragFloat3("Scale", &m_Data.transform->scale[0], 0.1f);
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