#pragma once
#include <vector>
#include <GLAD/glad.h>

enum class VertexFormat {
    LitTextured, // 8 floats per vertex: pos + uv + normal
    Colored      // 6 floats per vertex: pos + color
};

class Mesh {
public:
    Mesh(
        const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices,
        VertexFormat format = VertexFormat::LitTextured,
        GLenum mode = GL_TRIANGLES
    );
    ~Mesh();

    void Draw() const;

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
    unsigned int vertexCount;
    bool useEBO;
    GLenum drawMode;
    VertexFormat format;
};
