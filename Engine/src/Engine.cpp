#include "Engine.h"
#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

Engine::Engine(int width, int height, const std::string& title) {
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) { glfwTerminate(); throw std::runtime_error("Failed to create GLFW window"); }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    InitGL();

    // Camera
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Engine::~Engine() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Engine::InitGL() {
    glViewport(0, 0, 800, 600);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

glm::mat4 Engine::GetCameraProjectionMatrix() const {
    return glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
}

void Engine::ProcessInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Movement keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->ProcessKeyboard(RIGHT, deltaTime);
}

void Engine::Run(const std::function<void()>& renderCallback) {
    while (!glfwWindowShouldClose(window)) {
        // Frame timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Call Sandbox render code
        renderCallback();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// Mouse callback
void Engine::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (!engine) return;

    if (engine->firstMouse) {
        engine->lastX = xpos;
        engine->lastY = ypos;
        engine->firstMouse = false;
    }

    float xoffset = xpos - engine->lastX;
    float yoffset = engine->lastY - ypos;

    engine->lastX = xpos;
    engine->lastY = ypos;

    // Only rotate camera if right mouse button is pressed
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        engine->camera->ProcessMouseMovement(xoffset, yoffset);
}

// Scroll callback
void Engine::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (!engine) return;
    engine->camera->ProcessMouseScroll(yoffset);
}
