#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <string>
#include "Texture.h"
#include <Camera.h>

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

    Camera* camera;
    float lastX = 400.0f; // half of 800 width
    float lastY = 300.0f; // half of 600 height
    bool firstMouse = true;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;


    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
