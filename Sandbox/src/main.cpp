#include "Engine.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Light.h"
#include "Primitives.h"
#include "SceneObject.h"

#include <vector>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
    try {
        Engine engine(800, 600, "MarieEngine");

        auto cubeMesh = std::make_shared<Mesh>(CreateCube());
        auto texture = std::make_shared<Texture>("C:\\MarieEngine\\Engine\\Textures\\example.png");
        auto sharedMaterial = std::make_shared<Material>(texture);

        std::vector<SceneObject> sceneObjects;

        SceneObject cube1(cubeMesh, sharedMaterial);
        cube1.position = glm::vec3(-1.0f, 0.0f, 0.0f);
        sceneObjects.push_back(cube1);

        SceneObject cube2(cubeMesh, sharedMaterial);
        cube2.position = glm::vec3(1.0f, 0.0f, 0.0f);
        cube2.scale = glm::vec3(0.7f);
        sceneObjects.push_back(cube2);



        Light light({ 0,0,1 }, { 1,1,1 }, 0.1f, 1.0f, 1.0f);
        engine.RegisterEditableLight(&light);

        Shader shader(
            "C:\\MarieEngine\\Engine\\Shaders\\triangle.vert",
            "C:\\MarieEngine\\Engine\\Shaders\\triangle.frag"
        );


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

            for (const auto& object : sceneObjects) {
                object.Draw(shader);
            }
            //transformGizmo.Draw();
        });
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}