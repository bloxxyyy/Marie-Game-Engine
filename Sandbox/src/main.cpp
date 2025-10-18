#include "Engine.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Light.h"
#include "Primitives.h"

#include <vector>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ECS/ECSRegistry.h>
#include <ECS/Components.h>
#include <ECS/RenderSystem.h>

int main() {
    try {
        Engine engine(800, 600, "MarieEngine");

        ECSRegistry& registry = engine.GetRegistry();

        auto cubeMesh = std::make_shared<Mesh>(CreateCube());
        auto texture = std::make_shared<Texture>("C:\\MarieEngine\\Engine\\Textures\\example.png");
        auto sharedMaterial = std::make_shared<Material>(texture);

        Entity cube1 = registry.CreateEntity();
        registry.AddComponent(cube1, TagComponent{ "Cube A" });
        registry.AddComponent(cube1, RenderComponent{ cubeMesh, sharedMaterial });
        registry.AddComponent(cube1, TransformComponent{ {-1.0f, 0.0f, 0.0f} });

        Entity cube2 = registry.CreateEntity();
        registry.AddComponent(cube2, TagComponent{ "Cube B" });
        registry.AddComponent(cube2, RenderComponent{ cubeMesh, sharedMaterial });
        registry.AddComponent(cube2, TransformComponent{ {1.0f, 0.0f, 0.0f}, {}, {0.7f, 0.7f, 0.7f} });

        //Entity cube3 = registry.CreateEntity();
        //registry.AddComponent(cube3, TagComponent{ "Hidden Cube" });
        //registry.AddComponent(cube3, TransformComponent{ {1.0f, 0.0f, 0.0f}, {}, {0.7f, 0.7f, 0.7f} });

        Light light({ 0,0,1 }, { 1,1,1 }, 0.1f, 1.0f, 1.0f);
        engine.RegisterEditableLight(&light);

        Shader shader(
            "C:\\MarieEngine\\Engine\\Shaders\\triangle.vert",
            "C:\\MarieEngine\\Engine\\Shaders\\triangle.frag"
        );

        registry.RegisterSystem<RenderSystem>(shader);


        //shader.Use();
        //light.ApplyToShader(shader);
        //material.ApplyToShader(shader);


        //glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);

        /*
        Mesh transformGizmo = CreateGizmoAxes(1.0f);
        Shader shader(
            "C:\\MarieEngine\\Engine\\Shaders\\gizmo.vert",
            "C:\\MarieEngine\\Engine\\Shaders\\gizmo.frag"
        );
        shader.Use();
        */


        // Run the engine loop, pass a lambda to render each frame
        engine.Run([&]() {
            //texture.Bind(GL_TEXTURE0);
            shader.Use();
            light.ApplyToShader(shader);

            // Get view and projection from engine's camera
            glm::mat4 view = engine.GetCameraViewMatrix();
            glm::mat4 projection = engine.GetCameraProjectionMatrix();
            shader.SetMat4("view", view);
            shader.SetMat4("projection", projection);
            shader.SetVec3("viewPos", engine.GetCamera()->Position);
            
            auto& cube1Transform = registry.GetComponent<TransformComponent>(cube1);
            cube1Transform.rotation.y += 0.5f;

            auto& cube2Transform = registry.GetComponent<TransformComponent>(cube2);
            cube2Transform.rotation.x += 0.5f;
               
            //transformGizmo.Draw();
        });
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}