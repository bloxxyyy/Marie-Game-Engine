#pragma once
#include <string>
#include <glad/glad.h>

class Texture {
public:
    GLuint ID;
    int width, height, nrChannels;

    Texture(const std::string& path, bool flipVertically = true);
    ~Texture();

    void Bind(GLenum textureUnit = GL_TEXTURE0) const;
};
