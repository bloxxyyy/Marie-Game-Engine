#include "Engine.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
    try {
        Engine engine(800, 600, "MarieEngine");

        // Rectangle vertex data: pos(3) + color(3) + uv(2)
        std::vector<float> vertices = {
            // positions          // colors          // texture coords
            // Front face
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f,0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f,0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f,1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,1.0f,

            // Back face                      
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f,0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f,1.0f,

            // Left face
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f,0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f,0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f,1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f,1.0f,

            // Right face                     
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f,0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f,0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f,1.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f,1.0f,

             // Bottom face
             -0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f,  0.0f,1.0f,
              0.5f, -0.5f, -0.5f,  0.0f,1.0f,0.0f,  1.0f,1.0f,
              0.5f, -0.5f,  0.5f,  0.0f,0.0f,1.0f,  1.0f,0.0f,
             -0.5f, -0.5f,  0.5f,  1.0f,1.0f,0.0f,  0.0f,0.0f,

             // Top face
             -0.5f,  0.5f, -0.5f,  1.0f,0.0f,1.0f,  0.0f,1.0f,
              0.5f,  0.5f, -0.5f,  0.0f,1.0f,1.0f,  1.0f,1.0f,
              0.5f,  0.5f,  0.5f,  1.0f,1.0f,1.0f,  1.0f,0.0f,
             -0.5f,  0.5f,  0.5f,  0.0f,0.0f,0.0f,  0.0f,0.0f
        };

        std::vector<unsigned int> indices = {
            0,1,2, 2,3,0,       // front
            4,5,6, 6,7,4,       // back
            8,9,10,10,11,8,     // left
            12,13,14,14,15,12,  // right
            16,17,18,18,19,16,  // bottom
            20,21,22,22,23,20   // top
        };

        Mesh mesh(vertices, indices);
        Texture texture("C:\\MarieEngine\\Engine\\Textures\\example.png");
        Shader shader(
            "C:\\MarieEngine\\Engine\\Shaders\\triangle.vert",
            "C:\\MarieEngine\\Engine\\Shaders\\triangle.frag"
        );

        shader.Use();
        glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);

        // Run the engine loop, pass a lambda to render each frame
        engine.Run([&]() {
            texture.Bind(GL_TEXTURE0);
            shader.Use();

            // Model transform
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
                glm::vec3(0.5f, 1.0f, 0.0f));

            // Get view and projection from engine's camera
            glm::mat4 view = engine.GetCameraViewMatrix();
            glm::mat4 projection = engine.GetCameraProjectionMatrix();

            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            mesh.Draw();
        });
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
