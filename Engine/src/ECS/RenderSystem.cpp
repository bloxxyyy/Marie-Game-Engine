#include "ECS/RenderSystem.h"
#include "ECS/ECSRegistry.h"
#include "ECS/Components.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"

void RenderSystem::SetCameraData(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos) {
    mViewMatrix = view;
    mProjectionMatrix = projection;
    mViewPos = viewPos;
}

void RenderSystem::Update(ECSRegistry& registry, double deltaTime) {
    const auto& transformMap = registry.GetComponentMap<TransformComponent>();
    auto& renderableMap = registry.GetComponentMap<RenderComponent>();
    auto& lightMap = registry.GetComponentMap<LightComponent>();

    const LightComponent* activeLight = nullptr;
    const TransformComponent* lightTransform = nullptr;
    
    if (!lightMap.empty()) {
        auto& [lightEntity, lightComp] = *lightMap.begin();
        if (transformMap.contains(lightEntity)) {
            activeLight = &lightComp;
            lightTransform = &transformMap.at(lightEntity);
        }
    }
    
    for (auto const& [entity, renderComp] : renderableMap) {
        if (transformMap.contains(entity) && renderComp.material && renderComp.mesh) {
            auto& transformComp = transformMap.at(entity);
            const auto shader = renderComp.material->GetShader();
            
            renderComp.material->Apply();
            
            shader->SetMat4("view", mViewMatrix);
            shader->SetMat4("projection", mProjectionMatrix);
            shader->SetVec3("viewPos", mViewPos);
            
            // OpenGL safely ignores these if the shader doesn't use them
            if (activeLight && lightTransform) {
                shader->SetVec3("lightPos", lightTransform->position);
                shader->SetVec3("lightColor", activeLight->color);
                shader->SetFloat("ambientStrength", activeLight->ambientStrength);
                shader->SetFloat("diffuseStrength", activeLight->diffuseStrength);
                shader->SetFloat("specularStrength", activeLight->specularStrength);
                shader->SetFloat("shininess", activeLight->shininess);
            }
            
            shader->SetMat4("model", transformComp.GetModelMatrix());
            
            renderComp.mesh->Draw();
        }
    }
}