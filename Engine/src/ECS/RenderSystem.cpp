#include "ECS/ECSRegistry.h"
#include <ECS/RenderSystem.h>
#include <ECS/Components.h>

#include "Shader.h"
#include "Material.h" // Needed for material->ApplyToShader
#include "Mesh.h"     // Needed for mesh->Draw

RenderSystem::RenderSystem(Shader& shader) : m_Shader(shader) {}

void RenderSystem::Update(ECSRegistry& registry, float deltaTime) {
    m_Shader.Use();

    // --- Find and Set Light Uniforms ---
    // For this POC, we'll find the first entity with a LightComponent.
    // A more advanced engine would loop through multiple lights.
    auto& lightMap = registry.GetComponentMap<LightComponent>();
    auto& transformMap = registry.GetComponentMap<TransformComponent>();

    if (!lightMap.empty()) {
        // Get the first light entity and its component
        auto& [lightEntity, lightComp] = *lightMap.begin();

        // A light must also have a position
        if (transformMap.count(lightEntity)) {
            auto& lightTransform = transformMap.at(lightEntity);

            m_Shader.SetVec3("lightPos", lightTransform.position);
            m_Shader.SetVec3("lightColor", lightComp.color);
            m_Shader.SetFloat("ambientStrength", lightComp.ambientStrength);
            m_Shader.SetFloat("diffuseStrength", lightComp.diffuseStrength);
            m_Shader.SetFloat("specularStrength", lightComp.specularStrength);
            m_Shader.SetFloat("shininess", lightComp.shininess);
        }
    }

    auto& renderableMap = registry.GetComponentMap<RenderComponent>();

    for (auto const& [entity, renderComp] : renderableMap) {
        if (transformMap.count(entity)) {
            auto& transformComp = transformMap.at(entity);

            if (renderComp.material) {
                renderComp.material->ApplyToShader(m_Shader);
            }

            m_Shader.SetMat4("model", transformComp.GetModelMatrix());

            if (renderComp.mesh) {
                renderComp.mesh->Draw();
            }
        }
    }
}