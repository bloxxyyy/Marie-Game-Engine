#include "Input.h"
#include <stdexcept>
#include <imgui.h>

Input* Input::instance = nullptr;
GLFWcursorposfun Input::prevCursorPosCallback = nullptr;
GLFWscrollfun Input::prevScrollCallback = nullptr;
GLFWkeyfun Input::prevKeyCallback = nullptr;
GLFWmousebuttonfun Input::prevMouseButtonCallback = nullptr;

void Input::Initialize(GLFWwindow* window) {
    if (!window) throw std::runtime_error("GLFW window is null in Input::Initialize");
    if (!instance) instance = new Input();
    instance->window = window;

    prevCursorPosCallback = glfwSetCursorPosCallback(window, MouseCallback);
    prevScrollCallback = glfwSetScrollCallback(window, ScrollCallback);
    prevKeyCallback = glfwSetKeyCallback(window, KeyCallback);
    prevMouseButtonCallback = glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

Input& Input::Get() {
    if (!instance) throw std::runtime_error("Input not initialized!");
    return *instance;
}

void Input::Update() {
    keyStates.clear();
    mouseButtonStates.clear();

    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
        keyStates[key] = glfwGetKey(window, key) == GLFW_PRESS;

    for (int btn = GLFW_MOUSE_BUTTON_1; btn <= GLFW_MOUSE_BUTTON_LAST; ++btn)
        mouseButtonStates[btn] = glfwGetMouseButton(window, btn) == GLFW_PRESS;
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
    deltaX = deltaY = 0.0;
}

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (prevCursorPosCallback) prevCursorPosCallback(window, xpos, ypos);

    Input* input = instance;
    if (!input) return;

    if (input->firstMouse) {
        input->lastX = xpos;
        input->lastY = ypos;
        input->firstMouse = false;
    }

    // Always update lastX/Y to avoid jumps
    double dx = xpos - input->lastX;
    double dy = input->lastY - ypos; // y inverted
    input->lastX = xpos;
    input->lastY = ypos;

    // Only accumulate deltas if right mouse is down AND ImGui is not capturing
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS &&
        !ImGui::GetIO().WantCaptureMouse)
    {
        input->deltaX += dx;
        input->deltaY += dy;
    }
}


void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (prevScrollCallback) prevScrollCallback(window, xoffset, yoffset);

    Input* input = instance;
    if (!input) return;

    input->scrollOffsetY += yoffset;
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (prevKeyCallback) prevKeyCallback(window, key, scancode, action, mods);
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (prevMouseButtonCallback) prevMouseButtonCallback(window, button, action, mods);
}
