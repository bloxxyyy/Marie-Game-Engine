#include "GUI/GuiManager.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GUI/Theme/DefaultTheme.h>
#include "GUI/Monitoring/MonitoringPanel.h"

GuiManager::GuiManager(GLFWwindow* window) : m_Window(window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.Fonts->AddFontDefault();
    DefaultTheme::RegularFont = io.Fonts->AddFontFromFileTTF("C:\\DevProjects\\Marie-Game-Engine\\Engine\\Fonts\\Roboto-Regular.ttf", 20.0f);
    DefaultTheme::TopBarFont = io.Fonts->AddFontFromFileTTF("C:\\DevProjects\\Marie-Game-Engine\\Engine\\Fonts\\Roboto-Bold.ttf", 26.0f);
    DefaultTheme::HeaderFont = io.Fonts->AddFontFromFileTTF("C:\\DevProjects\\Marie-Game-Engine\\Engine\\Fonts\\Roboto-Bold.ttf", 22.0f);
    
    if (DefaultTheme::RegularFont)
        io.FontDefault = DefaultTheme::RegularFont;

    ImGui::StyleColorsDark();
    DefaultTheme::Apply();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

GuiManager::~GuiManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiManager::UpdateControllersPush() {
    for (auto& controller : m_Controllers) {
        controller->UpdatePush();
    }
}

void GuiManager::UpdateControllersPull() {
    for (auto& controller : m_Controllers) {
        controller->UpdatePull();
    }
}

void GuiManager::RenderViews() {
    BeginFrame();

    for (auto& controller : m_Controllers) {
        controller->GetView().OnImGuiRender();
    }

    EndFrame();
}

void GuiManager::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GuiManager::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}
