#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <string>
#include "Texture.h"

class Shader;
class Mesh;

class Engine {
public:
    Engine(int width, int height, const std::string& title);
    ~Engine();

    void Run();

private:
    void InitGL();

    GLFWwindow* window;
    Shader* shader;
    Mesh* mesh;
    Texture* texture;
};
