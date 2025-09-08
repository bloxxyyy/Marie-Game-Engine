#include "Input.h"
#include <stdexcept>

Input* Input::instance = nullptr;

void Input::Initialize(GLFWwindow* window) {
    if (!window) throw std::runtime_error("GLFW window is null in Input::Initialize");
    if (!instance) instance = new Input();
    instance->window = window;

    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

Input& Input::Get() {
    if (!instance) throw std::runtime_error("Input not initialized!");
    return *instance;
}

void Input::Update() {
    keyStates.clear();
    mouseButtonStates.clear();

    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
        keyStates[key] = glfwGetKey(window, key) == GLFW_PRESS;
    }

    for (int btn = GLFW_MOUSE_BUTTON_1; btn <= GLFW_MOUSE_BUTTON_LAST; ++btn) {
        mouseButtonStates[btn] = glfwGetMouseButton(window, btn) == GLFW_PRESS;
    }
}

bool Input::IsKeyDown(int key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

bool Input::IsMouseButtonDown(int button) const {
    auto it = mouseButtonStates.find(button);
    return it != mouseButtonStates.end() && it->second;
}

void Input::ResetDeltas() {
    deltaX = 0.0;
    deltaY = 0.0;
}

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Input* input = instance;
    if (!input) return;

    if (input->firstMouse) {
        input->lastX = xpos;
        input->lastY = ypos;
        input->firstMouse = false;
    }

    input->deltaX = xpos - input->lastX;
    input->deltaY = input->lastY - ypos; // y inverted

    input->lastX = xpos;
    input->lastY = ypos;
}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Input* input = instance;
    if (!input) return;

    input->scrollOffsetY += yoffset;
}
