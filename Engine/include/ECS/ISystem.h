#pragma once

class ECSRegistry;

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void Update(ECSRegistry& registry, float deltaTime) = 0;
};