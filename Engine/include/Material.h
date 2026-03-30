#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <string>
#include <variant>

#include "Shader.h"
#include "Texture.h"

// This variant can hold any standard shader uniform type
using MaterialParam = std::variant<float, int, glm::vec3, glm::mat4, std::shared_ptr<Texture>>;

class Material {
public:
    Material(std::shared_ptr<Shader> shader);
    void Set(const std::string& name, MaterialParam value);
    void Apply() const;
    std::shared_ptr<Shader> GetShader() const { return m_Shader; }

private:
    std::shared_ptr<Shader> m_Shader;
    std::map<std::string, MaterialParam> m_Parameters;
};