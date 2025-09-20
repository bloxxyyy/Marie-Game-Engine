#include "Material.h"

void Material::ApplyToShader(const Shader& shader) const {
    shader.SetVec3("ourColor", baseColor);

    if (texture) {
        glActiveTexture(GL_TEXTURE0);
        texture->Bind(GL_TEXTURE0);
        shader.SetInt("texture1", 0); // sampler binding
    }
}