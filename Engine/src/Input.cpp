#include "Input.h"

Input::Input(GLFWwindow* window, Camera* camera) : window(window), camera(camera) {
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

void Input::ProcessInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->ProcessKeyboard(RIGHT, deltaTime);
}

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
    if (!input) return;

    float x = static_cast<float>(xpos);
    float y = static_cast<float>(ypos);

    if (input->firstMouse) {
        input->lastX = x;
        input->lastY = y;
        input->firstMouse = false;
    }

    float xoffset = x - input->lastX;
    float yoffset = input->lastY - y;

    input->lastX = x;
    input->lastY = y;

    // Rotate camera only if RMB is pressed
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        input->camera->ProcessMouseMovement(xoffset, yoffset);
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
    if (!input) return;

    input->camera->ProcessMouseScroll(static_cast<float>(yoffset));
}
