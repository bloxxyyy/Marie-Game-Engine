#include "Engine.h"
#include <glad/glad.h>
#include <stdexcept>
#include "Mesh.h"
#include <vector>
#include "Texture.h"
#include "Shader.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

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

    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Rectangle vertex data: pos(3) + color(3) + uv(2)
    std::vector<float> vertices = {
        // positions          // colors          // texture coords
        // Front face
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f,0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f,0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f,1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,1.0f,
                                          
        // Back face                      
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f,0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f,1.0f,

        // Left face
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f,0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f,0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f,1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f,1.0f,
                                          
        // Right face                     
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f,0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f,0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f,1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f,1.0f,

         // Bottom face
         -0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f,  0.0f,1.0f,
          0.5f, -0.5f, -0.5f,  0.0f,1.0f,0.0f,  1.0f,1.0f,
          0.5f, -0.5f,  0.5f,  0.0f,0.0f,1.0f,  1.0f,0.0f,
         -0.5f, -0.5f,  0.5f,  1.0f,1.0f,0.0f,  0.0f,0.0f,

         // Top face
         -0.5f,  0.5f, -0.5f,  1.0f,0.0f,1.0f,  0.0f,1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,1.0f,1.0f,  1.0f,1.0f,
          0.5f,  0.5f,  0.5f,  1.0f,1.0f,1.0f,  1.0f,0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,0.0f,0.0f,  0.0f,0.0f
    };

    std::vector<unsigned int> indices = {
        0,1,2, 2,3,0,       // front
        4,5,6, 6,7,4,       // back
        8,9,10,10,11,8,     // left
        12,13,14,14,15,12,  // right
        16,17,18,18,19,16,  // bottom
        20,21,22,22,23,20   // top
    };


    mesh = new Mesh(vertices, indices);

    texture = new Texture("C:\\MarieEngine\\Engine\\Textures\\example.png");

    shader = new Shader(
        "C:\\MarieEngine\\Engine\\Shaders\\triangle.vert",
        "C:\\MarieEngine\\Engine\\Shaders\\triangle.frag"
    );

    shader->Use();
    glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0);
}

void Engine::InitGL() {
    glViewport(0, 0, 800, 600);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

Engine::~Engine() {
    delete mesh;
    delete shader;
    delete texture;
    delete camera;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Engine::Run() {
    while (!glfwWindowShouldClose(window)) {

        // Per-frame timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->ProcessKeyboard(RIGHT, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind texture and shader
        texture->Bind(GL_TEXTURE0);
        shader->Use();

        // Model: rotate cube over time
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, currentFrame * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // View matrix from camera pointer
        glm::mat4 view = camera->GetViewMatrix();

        // Projection: perspective
        glm::mat4 projection = glm::perspective(
            glm::radians(camera->Zoom), // FOV from camera
            800.0f / 600.0f,            // aspect ratio
            0.1f, 100.0f
        );

        // Send matrices to shader
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw mesh
        mesh->Draw();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void Engine::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

    if (engine->firstMouse) {
        engine->lastX = xpos;
        engine->lastY = ypos;
        engine->firstMouse = false;
    }

    float xoffset = xpos - engine->lastX;
    float yoffset = engine->lastY - ypos; // reversed since y-coordinates go from bottom to top

    engine->lastX = xpos;
    engine->lastY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        engine->camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

void Engine::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    engine->camera->ProcessMouseScroll(yoffset);
}