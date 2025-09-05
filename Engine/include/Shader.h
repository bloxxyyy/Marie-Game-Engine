#pragma once
#include <GLAD/glad.h>
#include <string>

class Shader {
public:
    GLuint ID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Use() const;

private:
    std::string LoadShaderSource(const std::string& path);
    GLuint CompileShader(GLenum type, const std::string& source);
    GLuint CreateShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
};
