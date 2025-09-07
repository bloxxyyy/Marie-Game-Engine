#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "Camera.h"
#include <string> 
#include <memory>

class Engine {
public:
    Engine(int width, int height, const std::string& title);
    ~Engine();

    // Run the engine loop, passing a lambda that renders the frame
    void Run(const std::function<void()>& renderCallback);

    // Camera accessors
    Camera* GetCamera() { return camera.get(); }
    glm::mat4 GetCameraViewMatrix() const { return camera->GetViewMatrix(); }
    glm::mat4 GetCameraProjectionMatrix() const;

private:
    void InitGL();
    void ProcessInput();

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    GLFWwindow* window = nullptr;
    std::unique_ptr<Camera> camera;

    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};
