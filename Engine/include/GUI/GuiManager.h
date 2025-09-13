#pragma once

#include <vector>
#include <memory>
#include "GUI/Panels/IPanel.h"
#include <GLFW/glfw3.h>

class GuiManager
{
public:
    GuiManager(GLFWwindow* window);
    ~GuiManager();

    void BeginFrame();
    void RenderPanels();
    void EndFrame();

    template<typename T, typename... Args>
    T* AddPanel(Args&&... args)
    {
        auto panel = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = panel.get();
        m_Panels.emplace_back(std::move(panel));
        return ptr;
    }

private:
    GLFWwindow* m_Window;
    std::vector<std::unique_ptr<IPanel>> m_Panels;
};
