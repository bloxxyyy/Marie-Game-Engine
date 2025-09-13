#include "Engine.h"
#include "Input.h"

#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

#include <GUI/Panels/MonitoringPanel.h>

Engine::Engine(int width, int height, const std::string& title) {
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) { glfwTerminate(); throw std::runtime_error("Failed to create GLFW window"); }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    InitGL();

    Input::Initialize(window);

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

    // Initialize GUI manager and panels
    stats = std::make_unique<EngineStats>();
    guiManager = std::make_unique<GuiManager>(window);
    guiManager->AddPanel<MonitoringPanel>(*stats);
}

Engine::~Engine() {
    guiManager.reset();
    stats.reset();

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

void Engine::Run(const std::function<void()>& renderCallback) {
    float fpsAccumulator = 0.0f;
    int frameCount = 0;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Update EngineStats
        stats->deltaTime = deltaTime;
        stats->fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
        fpsAccumulator += stats->fps;
        frameCount++;
        stats->avgFPS = fpsAccumulator / frameCount;

        // Stub CPU/GPU/memory usage for now
        stats->cpuUsage = 20.0f + (rand() % 50);
        stats->gpuUsage = 10.0f + (rand() % 80);
        stats->memoryUsed = 512.0f + (rand() % 1024);
        stats->memoryTotal = 4096.0f;

        // Start GUI frame
        guiManager->BeginFrame();
        guiManager->RenderPanels();
        guiManager->EndFrame();
        glfwMakeContextCurrent(window);

        // Input handling
        Input::Get().Update();

        if (Input::Get().IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera->ProcessMouseMovement(Input::Get().GetDeltaX(), Input::Get().GetDeltaY());
            Input::Get().ResetDeltas();
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        camera->Update(deltaTime);
        Input::Get().ResetDeltas();

        if (Input::Get().IsKeyDown(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, true);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderCallback();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
