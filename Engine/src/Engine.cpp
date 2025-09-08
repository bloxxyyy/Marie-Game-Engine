#include "Engine.h"
#include "Input.h"
#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
    InitImgui();

    Input::Initialize(window);

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
}

Engine::~Engine() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Engine::InitImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
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
    int   frameCount     = 0;
    float avgFPS         = 0.0f;

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float fps = 1.0f / deltaTime; // Instant FPS
        fpsAccumulator += fps; // A everage FPS
        frameCount++;
        avgFPS = fpsAccumulator / frameCount;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ---------------------------
        // Monitoring Window
        // ---------------------------
        {
            ImGui::Begin("Monitoring Window");

            ImGui::Text("Instant FPS: %.1f", fps);
            ImGui::Text("Avg FPS: %.1f", avgFPS);
            ImGui::Text("Frame time: %.3f ms", deltaTime * 1000.0f);

            ImGui::End();
        }

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

        // Escape key handled here (engine-level exit)
        if (Input::Get().IsKeyDown(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, true);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCallback();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update additional viewports if enabled
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(window);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
