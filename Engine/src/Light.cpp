#include "Light.h"

void Light::ApplyToShader(const Shader& shader) const {
    shader.SetVec3("lightPos", position);
    shader.SetVec3("lightColor", color);
    shader.SetFloat("ambientStrength", ambientStrength);
    shader.SetFloat("diffuseStrength", diffuseStrength);
    shader.SetFloat("specularStrength", specularStrength);
    shader.SetFloat("shininess", shininess);
}
