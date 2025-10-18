#pragma once
#include "ISystem.h"

class Shader;

class RenderSystem : public ISystem {
public:
    RenderSystem(Shader& shader);
    void Update(ECSRegistry& registry, float deltaTime) override;

private:
    Shader& m_Shader;
};