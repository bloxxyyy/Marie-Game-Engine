#include "GUI/Panels/EntitiesPanel.h"
#include "imgui.h"

EntitiesPanel::EntitiesPanel(DataType& data) : m_Data(data) {}

void EntitiesPanel::OnImGuiRender()
{
    if (!IsOpen) return;

    ImGui::Begin("Entities", &IsOpen);

    for (const auto& entityInfo : m_Data.entities)
    {
        bool isSelected = (m_Data.selectedEntity == entityInfo.id);
        std::string label = entityInfo.tag + " (ID: " + std::to_string(entityInfo.id) + ")";

        if (ImGui::Selectable(label.c_str(), isSelected))
        {
            if (isSelected) {
                m_Data.selectedEntity = NULL_ENTITY;
            }
            else {
                m_Data.selectedEntity = entityInfo.id;
            }
        }
    }

    ImGui::End();
}