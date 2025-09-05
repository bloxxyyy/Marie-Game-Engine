#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Engine {
public:
    Engine(int width = 800, int height = 600, const std::string& title = "MarieEngine");
    ~Engine();

    void Run(); // Main loop

private:
    GLFWwindow* window;

    GLuint VAO, VBO;
    GLuint shaderProgram;

    void InitGL();
    void SetupTriangle();
    void LoadShaders(const std::string& vertPath, const std::string& fragPath);
    std::string LoadShaderSource(const std::string& path);
    GLuint CompileShader(GLenum type, const std::string& source);
    GLuint CreateShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
};
