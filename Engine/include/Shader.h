#pragma once
#include <GLAD/glad.h>
#include <string>
#include <glm.hpp>

class Shader {
public:
    GLuint ID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Use() const;

    void SetInt(const std::string& name, int value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    std::string LoadShaderSource(const std::string& path);
    GLuint CompileShader(GLenum type, const std::string& source);
    GLuint CreateShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
};
