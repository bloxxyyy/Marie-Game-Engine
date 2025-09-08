#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <string>
#include <memory>

#include "Camera.h"
#include "Input.h"

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

private:
    void InitGL();
    void InitImgui();

    GLFWwindow* window = nullptr;

    std::unique_ptr<Input> input;
    std::unique_ptr<Camera> camera;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};
