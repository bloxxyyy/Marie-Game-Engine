#include "Engine.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Material.h>
#include <Light.h>
#include <Primitives.h>

int main() {
    try {
        Engine engine(800, 600, "MarieEngine");

        /*
        Mesh cube = CreateCube();
        Texture texture("C:\\MarieEngine\\Engine\\Textures\\example.png");
        Material material(texture);
        Light light({ 0,0,1 }, { 1,1,1 }, 0.1f, 1.0f, 1.0f);

        Shader shader(
            "C:\\MarieEngine\\Engine\\Shaders\\triangle.vert",
            "C:\\MarieEngine\\Engine\\Shaders\\triangle.frag"
        );


        shader.Use();
        light.ApplyToShader(shader);
        material.ApplyToShader(shader);
        */

        //glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);

		Mesh transformGizmo = CreateGizmoAxes(1.0f);
        Shader shader(
            "C:\\MarieEngine\\Engine\\Shaders\\gizmo.vert",
            "C:\\MarieEngine\\Engine\\Shaders\\gizmo.frag"
        );
        shader.Use();


        // Run the engine loop, pass a lambda to render each frame
        engine.Run([&]() {
            //texture.Bind(GL_TEXTURE0);
            shader.Use();
            //light.ApplyToShader(shader);
            //material.ApplyToShader(shader);

            // Model transform
            glm::mat4 model = glm::mat4(1.0f);
            //model = glm::rotate(model, (float)glfwGetTime() /** glm::radians(50.0f)*/,
            //    glm::vec3(/*0.5f*/0, 1.0f, 0.0f));
            /*
            model = glm::rotate(
                model,
                glm::radians(45.0f),
                glm::vec3(0, 1.0f, 0.0f)
            );*/

            // Get view and projection from engine's camera
            glm::mat4 view = engine.GetCameraViewMatrix();
            glm::mat4 projection = engine.GetCameraProjectionMatrix();

            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glm::vec3 viewPos = engine.GetCamera()->Position;
            glUniform3fv(glGetUniformLocation(shader.ID, "viewPos"), 1, glm::value_ptr(viewPos));

            //cube.Draw();
			transformGizmo.Draw();
        });
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
