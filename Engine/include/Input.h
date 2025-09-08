#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

class Input {
public:
    static void Initialize(GLFWwindow* window);
    static Input& Get();

    void Update();

    bool IsKeyDown(int key) const;
    bool IsMouseButtonDown(int button) const;

    double GetDeltaX() const { return deltaX; }
    double GetDeltaY() const { return deltaY; }
    void ResetDeltas();

    double GetScrollOffsetY() const { return scrollOffsetY; }
    void ResetScrollOffset() { scrollOffsetY = 0.0; }

private:
    Input() = default;

    static Input* instance;

    GLFWwindow* window = nullptr;

    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, bool> mouseButtonStates;

    double lastX = 0.0;
    double lastY = 0.0;
    double deltaX = 0.0;
    double deltaY = 0.0;
    bool firstMouse = true;

    double scrollOffsetY = 0.0;

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
