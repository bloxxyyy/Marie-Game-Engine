#include "Engine.h"
#include <glad/glad.h>
#include <stdexcept>
#include "Mesh.h"
#include <vector>
#include "Texture.h"
#include "Shader.h"

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

    // Rectangle vertex data: pos(3) + color(3) + uv(2)
    std::vector<float> vertices = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

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
}

Engine::~Engine() {
    delete mesh;
    delete shader;
    delete texture;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Engine::Run() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        texture->Bind(GL_TEXTURE0);
        shader->Use();
        mesh->Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
