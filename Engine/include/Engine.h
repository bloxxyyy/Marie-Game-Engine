#pragma once

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

// GLAD before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <string>
#include <memory>

#include "Camera.h"
#include "Input.h"
#include "GUI/Data/EngineStats.h"
#include "GUI/GuiManager.h"
#include "Monitoring/Win_Monitoring.h"
#include <ECS/ECSRegistry.h>

class Engine {
public:
    Engine(int width, int height, const std::string& title);
    ~Engine();

    void Run(const std::function<void()>& renderCallback);

    Input* GetInput() { return input.get(); }
    Camera* GetCamera() { return camera.get(); }

    glm::mat4 GetCameraViewMatrix() const { return camera->GetViewMatrix(); }
    glm::mat4 GetCameraProjectionMatrix() const;

    float GetDeltaTime() const { return deltaTime; }

    ECSRegistry& GetRegistry() { return *m_Registry; }

private:
    void InitGL();

    GLFWwindow* window = nullptr;

    std::unique_ptr<Input> input;
    std::unique_ptr<Camera> camera;

    std::unique_ptr<ECSRegistry> m_Registry;

    std::unique_ptr<EngineStats> stats;
    std::unique_ptr<GuiManager> guiManager;
    std::unique_ptr<Win_Monitoring> monitorManager;

    float deltaTime = 0.0f;
};