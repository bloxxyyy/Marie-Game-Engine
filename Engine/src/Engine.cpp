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
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Engine::Run() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture->Bind(GL_TEXTURE0);
        shader->Use();

        // Model: rotate over time
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // View: move "camera" backwards
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        // Projection: perspective
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), // FOV
            800.0f / 600.0f,     // aspect ratio
            0.1f, 100.0f         // near and far planes
        );

        // Send matrices to shader
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


        mesh->Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
