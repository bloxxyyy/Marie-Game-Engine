#pragma once
#include <string>
#include <glad/glad.h>

class Texture {
public:
    GLuint ID = 0;
    int width, height, nrChannels;

    Texture(const std::string& path, bool flipVertically = true);
    ~Texture();

    // Rule of Five del the copy constructor and copy assignment operator
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // move constructor and move assignment operator
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void Bind(GLenum textureUnit = GL_TEXTURE0) const;
};