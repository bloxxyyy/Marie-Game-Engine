#pragma once

#include "ISystem.h"

class ECSRegistry;
class Shader;

class RenderSystem : public ISystem {
public:
    void Update(ECSRegistry& registry, Shader& shader);
};