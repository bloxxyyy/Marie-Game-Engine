#include "Engine.h"
#include "Input.h"

#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

#include <GUI/GuiManager.h>
#include <GUI/Data/LightData.h>
#include <GUI/Panels/LightEditorPanel.h>
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
    m_Registry = std::make_unique<ECSRegistry>();

    stats = std::make_unique<EngineStats>();
    guiManager = std::make_unique<GuiManager>(window);
    monitorManager = std::make_unique<Win_Monitoring>();

    guiManager->AddPanel<LightEditorPanel>();
    guiManager->AddPanel<MonitoringPanel>();
}

Engine::~Engine() {
    guiManager.reset();
    stats.reset();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Engine::RegisterEditableLight(Light* light) {
    m_EditableLight = light;
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
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = static_cast<float>(glfwGetTime());
        FrameStats frameStats = monitorManager->UpdateFrameTiming(currentFrame);
        deltaTime = frameStats.deltaTime;
        stats->avgFPS = frameStats.avgFPS;
        stats->fps = frameStats.fps;
        stats->deltaTime = frameStats.deltaTime;
        stats->cpuUsage = monitorManager->GetCPUUsage(deltaTime);

        Input::Get().Update();
        if (Input::Get().IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera->ProcessMouseMovement(
                static_cast<float>(Input::Get().GetDeltaX()),
                static_cast<float>(Input::Get().GetDeltaY())
            );
            Input::Get().ResetDeltas();
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        camera->Update(deltaTime);
        Input::Get().ResetDeltas();
        if (Input::Get().IsKeyDown(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, true);

        if (m_EditableLight) {
            LightData currentLightData;
            currentLightData.position = m_EditableLight->position;
            currentLightData.color = m_EditableLight->color;
            currentLightData.ambientStrength = m_EditableLight->ambientStrength;
            currentLightData.diffuseStrength = m_EditableLight->diffuseStrength;
            currentLightData.specularStrength = m_EditableLight->specularStrength;
            currentLightData.shininess = m_EditableLight->shininess;
            guiManager->SetData<LightData>(currentLightData);
        }

        guiManager->SetData<EngineStats>(*stats);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderCallback();

        // Update all  systems
        m_Registry->Update(deltaTime);

        // GUI Rendering
        guiManager->BeginFrame();
        guiManager->RenderPanels();
        guiManager->EndFrame();

        if (m_EditableLight) {
            const LightData& modifiedLightData = guiManager->GetData<LightData>();
            m_EditableLight->position = modifiedLightData.position;
            m_EditableLight->color = modifiedLightData.color;
            m_EditableLight->ambientStrength = modifiedLightData.ambientStrength;
            m_EditableLight->diffuseStrength = modifiedLightData.diffuseStrength;
            m_EditableLight->specularStrength = modifiedLightData.specularStrength;
            m_EditableLight->shininess = modifiedLightData.shininess;
        }

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}