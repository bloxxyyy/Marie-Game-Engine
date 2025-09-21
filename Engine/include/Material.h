#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include <memory>

class Material {
public:
    glm::vec3 baseColor{ 1.0f, 1.0f, 1.0f };
    std::shared_ptr<const Texture> texture = nullptr;

    Material(std::shared_ptr<const Texture> tex) : texture(tex) {}

    void ApplyToShader(const Shader& shader) const;
};
