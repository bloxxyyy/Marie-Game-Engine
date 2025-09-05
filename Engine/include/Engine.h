#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

class Engine {
public:
    Engine(int width, int height, const std::string& title);
    ~Engine();
    void Run();

private:
    void InitGL();
    void SetupTriangle();

    GLFWwindow* window;
    GLuint VAO, VBO;
    Shader* shader; // replace previous GLuint shaderProgram
};
