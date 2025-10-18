#include <ECS/ECSRegistry.h>

ECSRegistry::ECSRegistry() {
    m_NextEntityID = 0;
}

Entity ECSRegistry::CreateEntity() {
    // TODO object pooling and recycling here.
    // For now, a simple incrementing ID.
    return m_NextEntityID++;
}

void ECSRegistry::Update(float deltaTime) {
    for (auto& system : m_Systems) {
        system->Update(*this, deltaTime);
    }
}