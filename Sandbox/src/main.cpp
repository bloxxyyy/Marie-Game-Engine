#include "Engine.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Primitives.h"

#include <iostream>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <ECS/ECSRegistry.h>
#include <ECS/Components.h>
#include <ECS/RenderSystem.h>

int main() {
    try {
        Engine engine(800, 600, "MarieEngine");

        ECSRegistry& registry = engine.GetRegistry();
        
        auto cubeMesh = std::make_shared<Mesh>(CreateCube());
        auto gizmoMesh = std::make_shared<Mesh>(CreateGizmoAxes(2.0f));
        
        auto texture = std::make_shared<Texture>(R"(C:\DevProjects\Marie-Game-Engine\Engine\Textures\example.png)");
        
        auto litShader = std::make_shared<Shader>(
            R"(C:\DevProjects\Marie-Game-Engine\Engine\Shaders\triangle.vert)",
            R"(C:\DevProjects\Marie-Game-Engine\Engine\Shaders\triangle.frag)"
        );
        
        auto gizmoShader = std::make_shared<Shader>(
            R"(C:\DevProjects\Marie-Game-Engine\Engine\Shaders\gizmo.vert)",
            R"(C:\DevProjects\Marie-Game-Engine\Engine\Shaders\gizmo.frag)"
        );
        
        auto litMaterial = std::make_shared<Material>(litShader);
        litMaterial->Set("texture1", texture);

        auto gizmoMaterial = std::make_shared<Material>(gizmoShader);
        
        Entity lightEntity = registry.CreateEntity();
        registry.AddComponent(lightEntity, TagComponent{ "Point Light" });
        registry.AddComponent(lightEntity, TransformComponent{ {0.0f, 0.0f, 0.0f} });
        registry.AddComponent(lightEntity, LightComponent{
            {1.0f, 1.0f, 1.0f}, // Color
            0.2f, 1.0f, 1.0f, 32.0f // ambient, diffuse, specular, shininess
            });

        Entity cube1 = registry.CreateEntity();
        registry.AddComponent(cube1, TagComponent{ "Cube A" });
        registry.AddComponent(cube1, RenderComponent{ cubeMesh, litMaterial });
        registry.AddComponent(cube1, TransformComponent{ {-1.0f, 0.0f, 0.0f} });

        Entity cube2 = registry.CreateEntity();
        registry.AddComponent(cube2, TagComponent{ "Cube B" });
        registry.AddComponent(cube2, RenderComponent{ cubeMesh, litMaterial });
        registry.AddComponent(cube2, TransformComponent{ {1.0f, 0.0f, 0.0f}, {}, {0.7f, 0.7f, 0.7f} });

        Entity gizmo = registry.CreateEntity();
        registry.AddComponent(gizmo, TagComponent{ "World Gizmo" });
        registry.AddComponent(gizmo, RenderComponent{ gizmoMesh, gizmoMaterial });
        registry.AddComponent(gizmo, TransformComponent{ {0.0f, 0.0f, 0.0f} });
 
        std::shared_ptr<RenderSystem> renderSystem = registry.RegisterSystem<RenderSystem>();
        
        // Run the engine loop, pass a lambda to render each frame
        engine.Run([&]() {

            renderSystem->SetCameraData(
                engine.GetCameraViewMatrix(), 
                engine.GetCameraProjectionMatrix(), 
                engine.GetCamera()->Position
            );
            
            auto& cube1Transform = registry.GetComponent<TransformComponent>(cube1);
            cube1Transform.rotation.y += 0.5f;

            auto& cube2Transform = registry.GetComponent<TransformComponent>(cube2);
            cube2Transform.rotation.x += 0.5f;
        });
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }

    return 0;
}