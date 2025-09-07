#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

class Input {
public:
    Input(GLFWwindow* window, Camera* camera);

    void ProcessInput(float deltaTime);

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    GLFWwindow* window;
    Camera* camera;

    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;
};
