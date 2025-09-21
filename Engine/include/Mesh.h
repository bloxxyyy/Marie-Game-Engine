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

    // Rule of Five del copy constructor and copy assignment
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Impl move constructor and move assignment
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void Draw() const;

private:
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    unsigned int indexCount;
    unsigned int vertexCount;
    bool useEBO;
    GLenum drawMode;
    VertexFormat format;
};