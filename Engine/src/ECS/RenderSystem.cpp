#include "ECS/ECSRegistry.h"
#include <ECS/RenderSystem.h>
#include <ECS/Components.h>

#include "Shader.h"
#include "Material.h" // Needed for material->ApplyToShader
#include "Mesh.h"     // Needed for mesh->Draw

RenderSystem::RenderSystem(Shader& shader) : m_Shader(shader) {}

void RenderSystem::Update(ECSRegistry& registry, float deltaTime) {
    auto& transformMap = registry.GetComponentMap<TransformComponent>();
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