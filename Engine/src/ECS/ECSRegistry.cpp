#include <ECS/ECSRegistry.h>

ECSRegistry::ECSRegistry() {
    m_NextEntityID = 0;
}

Entity ECSRegistry::CreateEntity() {
    // TODO object pooling and recycling here.
    // For now, a simple incrementing ID.
    return m_NextEntityID++;
}