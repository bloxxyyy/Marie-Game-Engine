#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh;
class Material;

struct TagComponent {
    std::string tag;
};

struct LightComponent {
    glm::vec3 color{ 1.0f, 1.0f, 1.0f };
    float ambientStrength = 0.1f;
    float diffuseStrength = 1.0f;
    float specularStrength = 1.0f;
    float shininess = 32.0f;
};

struct TransformComponent {
    glm::vec3 position{ 0.0f };
    glm::vec3 rotation{ 0.0f }; // Euler angles in degrees
    glm::vec3 scale{ 1.0f };

    glm::mat4 GetModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }
};

struct RenderComponent {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
};