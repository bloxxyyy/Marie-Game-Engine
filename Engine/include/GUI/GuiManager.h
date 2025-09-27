#pragma once

#include <vector>
#include <memory>
#include "GUI/Panels/IPanel.h"
#include <GLFW/glfw3.h>

#include <map>
#include <typeindex>
#include <any>

class GuiManager
{
public:
    GuiManager(GLFWwindow* window);
    ~GuiManager();

    void BeginFrame();
    void RenderPanels();
    void EndFrame();

    template<typename TPanel>
    TPanel* AddPanel()
    {
        using TData = typename TPanel::DataType;

        auto it = m_DataHub.find(std::type_index(typeid(TData)));
        if (it == m_DataHub.end())
        {
            m_DataHub[std::type_index(typeid(TData))] = TData{};
        }

        TData& dataCache = std::any_cast<TData&>(m_DataHub.at(std::type_index(typeid(TData))));

        auto panel = std::make_unique<TPanel>(dataCache);
        TPanel* ptr = panel.get();
        m_Panels.emplace_back(std::move(panel));
        return ptr;
    }

    template<typename TData>
    void SetData(const TData& data) {
        m_DataHub[std::type_index(typeid(TData))] = data;
    }

    template<typename TData>
    const TData& GetData() const {
        return std::any_cast<const TData&>(m_DataHub.at(std::type_index(typeid(TData))));
    }

private:
    GLFWwindow* m_Window;
    std::vector<std::unique_ptr<IPanel>> m_Panels;
    std::map<std::type_index, std::any> m_DataHub;
};