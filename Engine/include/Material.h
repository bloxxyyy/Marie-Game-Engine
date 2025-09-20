#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

class Material {
public:
    glm::vec3 baseColor{ 1.0f, 1.0f, 1.0f };
    const Texture* texture = nullptr;

    Material(const Texture& tex) : texture(&tex) {}

    void ApplyToShader(const Shader& shader) const;
};
