#pragma once
#include <glm/glm.hpp>

struct LightData {
    glm::vec3 position{ 0.0f };
    glm::vec3 color{ 1.0f, 1.0f, 1.0f };
    float ambientStrength = 0.1f;
    float diffuseStrength = 1.0f;
    float specularStrength = 1.0f;
    float shininess = 32.0f;
};