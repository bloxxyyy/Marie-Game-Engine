#pragma once
#include "ECS/ISystem.h"
#include <glm/glm.hpp>

class RenderSystem : public ISystem {
public:
    RenderSystem() = default;
    
    void SetCameraData(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos);

    void Update(ECSRegistry& registry, float deltaTime) override;

private:
    glm::mat4 mViewMatrix;
    glm::mat4 mProjectionMatrix;
    glm::vec3 mViewPos;
};