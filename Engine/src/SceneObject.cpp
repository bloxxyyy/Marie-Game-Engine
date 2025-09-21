#include "SceneObject.h"
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

SceneObject::SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
    : mesh(mesh), material(material)
{
    // Ensure that the pointers are valid upon creation.
    if (!this->mesh) {
        throw std::runtime_error("SceneObject must be initialized with a valid mesh.");
    }
    if (!this->material) {
        throw std::runtime_error("SceneObject must be initialized with a valid material.");
    }
}

glm::mat4 SceneObject::GetModelMatrix() const {

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    return model;
}

void SceneObject::Draw(Shader& shader) const {
    material->ApplyToShader(shader);
    shader.SetMat4("model", GetModelMatrix());
    mesh->Draw();
}