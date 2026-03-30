#include "Material.h"

Material::Material(std::shared_ptr<Shader> shader) : m_Shader(shader) {}

void Material::Set(const std::string& name, MaterialParam value) {
    m_Parameters[name] = value;
}

void Material::Apply() const {
    if (!m_Shader) return;

    m_Shader->Use();
    int textureUnit = 0;
    
    for (const auto& [name, value] : m_Parameters) {
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            
            if constexpr (std::is_same_v<T, float>)
                m_Shader->SetFloat(name, arg);
            else if constexpr (std::is_same_v<T, int>)
                m_Shader->SetInt(name, arg);
            else if constexpr (std::is_same_v<T, glm::vec3>)
                m_Shader->SetVec3(name, arg);
            else if constexpr (std::is_same_v<T, glm::mat4>)
                m_Shader->SetMat4(name, arg);
            else if constexpr (std::is_same_v<T, std::shared_ptr<Texture>>) {
                if (arg) {
                    arg->Bind(GL_TEXTURE0 + textureUnit);
                    m_Shader->SetInt(name, textureUnit);
                    textureUnit++;
                }
            }
        }, value);
    }
}