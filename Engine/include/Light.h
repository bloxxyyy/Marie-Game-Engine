#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class Light {
public:
    glm::vec3 position{ 0.0f, 0.0f, 1.0f };
    glm::vec3 color{ 1.0f, 1.0f, 1.0f };

    float ambientStrength = 0.1f;
    float diffuseStrength = 1.0f;
    float specularStrength = 1.0f;
    float shininess = 32.0f;

    Light(const glm::vec3& pos = { 0.0f, 0.0f, 1.0f }, const glm::vec3& col = { 1.0f, 1.0f, 1.0f }, float ambient = 0.1f, float diffuse = 1.0f,  float specular = 1.0f, float shiny = 32.0f) : position(pos), color(col), ambientStrength(ambient), diffuseStrength(diffuse), specularStrength(specular), shininess(shiny) { }

    void ApplyToShader(const Shader& shader) const;
};
