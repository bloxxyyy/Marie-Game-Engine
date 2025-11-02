#pragma once

#include <vector>
#include <memory>
#include "GUI/Panels/IPanelView.h"
#include <GLFW/glfw3.h>

#include <map>
#include <typeindex>
#include <any>
#include "Panels/IPanelController.h"

class GuiManager
{
public:
    GuiManager(GLFWwindow* window);
    ~GuiManager();

    void UpdateControllersPush();
    void UpdateControllersPull();
    void RenderViews();

    template<typename TController, typename... TArgs>
    TController* AddController(TArgs&&... args)
    {
        auto controller = std::make_unique<TController>(std::forward<TArgs>(args)...);
        TController* ptr = controller.get();
        m_Controllers.emplace_back(std::move(controller));
        return ptr;
    }

private:
    void BeginFrame();
    void EndFrame();
    GLFWwindow* m_Window;
    std::vector<std::unique_ptr<IPanelController>> m_Controllers;
};