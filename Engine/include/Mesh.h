#pragma once
#include <vector>

class Mesh {
public:
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void Draw() const;

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
    unsigned int vertexCount;
    bool useEBO;
};
