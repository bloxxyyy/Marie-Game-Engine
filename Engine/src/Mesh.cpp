#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices,
    VertexFormat fmt,
    GLenum mode)
    : format(fmt), drawMode(mode)
{
    useEBO = !indices.empty();
    vertexCount = static_cast<GLsizei>(vertices.size() /
        (format == VertexFormat::LitTextured ? 8 : 6));
    indexCount = static_cast<GLsizei>(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    if (useEBO) glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Upload indices if present
    if (useEBO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

    // Set up vertex attributes
    if (format == VertexFormat::LitTextured) {
        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Texcoords
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    else if (format == VertexFormat::Colored) {
        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (useEBO) glDeleteBuffers(1, &EBO);
}

void Mesh::Draw() const {
    glBindVertexArray(VAO);
    if (useEBO)
        glDrawElements(drawMode, indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(drawMode, 0, vertexCount);
    glBindVertexArray(0);
}
