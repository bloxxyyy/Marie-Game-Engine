#include "Texture.h"
#include "stb_image.h"
#include <stdexcept>
#include <iostream>

Texture::Texture(const std::string& path, bool flipVertically) {
    stbi_set_flip_vertically_on_load(flipVertically);

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) throw std::runtime_error("Failed to load texture: " + path);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture() {
    if (ID != 0) {
        glDeleteTextures(1, &ID);
    }
}

Texture::Texture(Texture&& other) noexcept: ID(other.ID), width(other.width), height(other.height), nrChannels(other.nrChannels)
{
    other.ID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {

        if (ID != 0) {
            glDeleteTextures(1, &ID);
        }

        ID = other.ID;
        width = other.width;
        height = other.height;
        nrChannels = other.nrChannels;

        // Invalidate the moved-from object
        other.ID = 0;
    }
    return *this;
}

void Texture::Bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
