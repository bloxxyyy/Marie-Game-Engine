#include "Primitives.h"
#include <vector>

Mesh CreateCube() {
    // pos(3) + uv(2) + normal(3) = 8 floats per vertex
    std::vector<float> vertices = {
        // positions        // tex coords // normals
        -0.5f,-0.5f, 0.5f,   0.0f,0.0f,   0.0f,0.0f,1.0f,
         0.5f,-0.5f, 0.5f,   1.0f,0.0f,   0.0f,0.0f,1.0f,
         0.5f, 0.5f, 0.5f,   1.0f,1.0f,   0.0f,0.0f,1.0f,
        -0.5f, 0.5f, 0.5f,   0.0f,1.0f,   0.0f,0.0f,1.0f,

        // back face
        -0.5f,-0.5f,-0.5f,   1.0f,0.0f,   0.0f,0.0f,-1.0f,
         0.5f,-0.5f,-0.5f,   0.0f,0.0f,   0.0f,0.0f,-1.0f,
         0.5f, 0.5f,-0.5f,   0.0f,1.0f,   0.0f,0.0f,-1.0f,
        -0.5f, 0.5f,-0.5f,   1.0f,1.0f,   0.0f,0.0f,-1.0f,

        // left face
        -0.5f,-0.5f,-0.5f,   0.0f,0.0f,  -1.0f,0.0f,0.0f,
        -0.5f,-0.5f, 0.5f,   1.0f,0.0f,  -1.0f,0.0f,0.0f,
        -0.5f, 0.5f, 0.5f,   1.0f,1.0f,  -1.0f,0.0f,0.0f,
        -0.5f, 0.5f,-0.5f,   0.0f,1.0f,  -1.0f,0.0f,0.0f,

        // right face
         0.5f,-0.5f,-0.5f,   1.0f,0.0f,   1.0f,0.0f,0.0f,
         0.5f,-0.5f, 0.5f,   0.0f,0.0f,   1.0f,0.0f,0.0f,
         0.5f, 0.5f, 0.5f,   0.0f,1.0f,   1.0f,0.0f,0.0f,
         0.5f, 0.5f,-0.5f,   1.0f,1.0f,   1.0f,0.0f,0.0f,

         // bottom face
         -0.5f,-0.5f,-0.5f,   0.0f,1.0f,   0.0f,-1.0f,0.0f,
          0.5f,-0.5f,-0.5f,   1.0f,1.0f,   0.0f,-1.0f,0.0f,
          0.5f,-0.5f, 0.5f,   1.0f,0.0f,   0.0f,-1.0f,0.0f,
         -0.5f,-0.5f, 0.5f,   0.0f,0.0f,   0.0f,-1.0f,0.0f,

         // top face
         -0.5f, 0.5f,-0.5f,   0.0f,1.0f,   0.0f,1.0f,0.0f,
          0.5f, 0.5f,-0.5f,   1.0f,1.0f,   0.0f,1.0f,0.0f,
          0.5f, 0.5f, 0.5f,   1.0f,0.0f,   0.0f,1.0f,0.0f,
         -0.5f, 0.5f, 0.5f,   0.0f,0.0f,   0.0f,1.0f,0.0f
    };

    std::vector<unsigned int> indices = {
        0,1,2, 2,3,0,       // front
        4,5,6, 6,7,4,       // back
        8,9,10,10,11,8,     // left
        12,13,14,14,15,12,  // right
        16,17,18,18,19,16,  // bottom
        20,21,22,22,23,20   // top
    };

    return Mesh(vertices, indices);
}
