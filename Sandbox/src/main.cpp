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

        // pos(3) + color(3) + uv(2) + normal(3) = 11 floats per vertex
        std::vector<float> vertices = {
            // positions          // colors        // tex coords // normals
            // Front face (0,0,1)
            -0.5f,-0.5f, 0.5f,    1.0f,0.0f,0.0f,  0.0f,0.0f,    0.0f,0.0f,1.0f,
             0.5f,-0.5f, 0.5f,    0.0f,1.0f,0.0f,  1.0f,0.0f,    0.0f,0.0f,1.0f,
             0.5f, 0.5f, 0.5f,    0.0f,0.0f,1.0f,  1.0f,1.0f,    0.0f,0.0f,1.0f,
            -0.5f, 0.5f, 0.5f,    1.0f,1.0f,0.0f,  0.0f,1.0f,    0.0f,0.0f,1.0f,

            // Back face (0,0,-1)
            -0.5f,-0.5f,-0.5f,    1.0f,0.0f,1.0f,  1.0f,0.0f,    0.0f,0.0f,-1.0f,
             0.5f,-0.5f,-0.5f,    0.0f,1.0f,1.0f,  0.0f,0.0f,    0.0f,0.0f,-1.0f,
             0.5f, 0.5f,-0.5f,    1.0f,1.0f,1.0f,  0.0f,1.0f,    0.0f,0.0f,-1.0f,
            -0.5f, 0.5f,-0.5f,    0.0f,0.0f,0.0f,  1.0f,1.0f,    0.0f,0.0f,-1.0f,

            // Left face (-1,0,0)
            -0.5f,-0.5f,-0.5f,    1.0f,0.0f,0.0f,  0.0f,0.0f,   -1.0f,0.0f,0.0f,
            -0.5f,-0.5f, 0.5f,    0.0f,1.0f,0.0f,  1.0f,0.0f,   -1.0f,0.0f,0.0f,
            -0.5f, 0.5f, 0.5f,    0.0f,0.0f,1.0f,  1.0f,1.0f,   -1.0f,0.0f,0.0f,
            -0.5f, 0.5f,-0.5f,    1.0f,1.0f,0.0f,  0.0f,1.0f,   -1.0f,0.0f,0.0f,

            // Right face (1,0,0)
             0.5f,-0.5f,-0.5f,    1.0f,0.0f,1.0f,  1.0f,0.0f,    1.0f,0.0f,0.0f,
             0.5f,-0.5f, 0.5f,    0.0f,1.0f,1.0f,  0.0f,0.0f,    1.0f,0.0f,0.0f,
             0.5f, 0.5f, 0.5f,    1.0f,1.0f,1.0f,  0.0f,1.0f,    1.0f,0.0f,0.0f,
             0.5f, 0.5f,-0.5f,    0.0f,0.0f,0.0f,  1.0f,1.0f,    1.0f,0.0f,0.0f,

             // Bottom face (0,-1,0)
             -0.5f,-0.5f,-0.5f,    1.0f,0.0f,0.0f,  0.0f,1.0f,    0.0f,-1.0f,0.0f,
              0.5f,-0.5f,-0.5f,    0.0f,1.0f,0.0f,  1.0f,1.0f,    0.0f,-1.0f,0.0f,
              0.5f,-0.5f, 0.5f,    0.0f,0.0f,1.0f,  1.0f,0.0f,    0.0f,-1.0f,0.0f,
             -0.5f,-0.5f, 0.5f,    1.0f,1.0f,0.0f,  0.0f,0.0f,    0.0f,-1.0f,0.0f,

             // Top face (0,1,0)
             -0.5f, 0.5f,-0.5f,    1.0f,0.0f,1.0f,  0.0f,1.0f,    0.0f,1.0f,0.0f,
              0.5f, 0.5f,-0.5f,    0.0f,1.0f,1.0f,  1.0f,1.0f,    0.0f,1.0f,0.0f,
              0.5f, 0.5f, 0.5f,    1.0f,1.0f,1.0f,  1.0f,0.0f,    0.0f,1.0f,0.0f,
             -0.5f, 0.5f, 0.5f,    0.0f,0.0f,0.0f,  0.0f,0.0f,    0.0f,1.0f,0.0f
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

        float diffuseStrength = 1.0f; // full diffuse

        // Light settings
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos(1.2f, .5f, 2.0f); // fixed light position in world space
        float ambientStrength = 0.1f;

        // Run the engine loop, pass a lambda to render each frame
        engine.Run([&]() {
            texture.Bind(GL_TEXTURE0);
            shader.Use();

            glUniform3fv(glGetUniformLocation(shader.ID, "lightColor"), 1, glm::value_ptr(lightColor));
            glUniform3fv(glGetUniformLocation(shader.ID, "lightPos"), 1, glm::value_ptr(lightPos));
            glUniform1f(glGetUniformLocation(shader.ID, "ambientStrength"), ambientStrength);
            glUniform1f(glGetUniformLocation(shader.ID, "diffuseStrength"), diffuseStrength);
            glUniform1f(glGetUniformLocation(shader.ID, "specularStrength"), 1.0f);
            glUniform1f(glGetUniformLocation(shader.ID, "shininess"), 32.0f);

            // Model transform
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() /** glm::radians(50.0f)*/,
                glm::vec3(/*0.5f*/0, 1.0f, 0.0f));

            // Get view and projection from engine's camera
            glm::mat4 view = engine.GetCameraViewMatrix();
            glm::mat4 projection = engine.GetCameraProjectionMatrix();

            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glm::vec3 viewPos = engine.GetCamera()->Position;
            glUniform3fv(glGetUniformLocation(shader.ID, "viewPos"), 1, glm::value_ptr(viewPos));

            mesh.Draw();
        });
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
