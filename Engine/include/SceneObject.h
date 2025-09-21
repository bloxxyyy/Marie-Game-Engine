#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class SceneObject {
public:
    glm::vec3 position{ 0.0f };
    glm::vec3 rotation{ 0.0f }; // Euler angles in degrees
    glm::vec3 scale{ 1.0f };

    SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
    glm::mat4 GetModelMatrix() const;
    void Draw(Shader& shader) const;

private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
};