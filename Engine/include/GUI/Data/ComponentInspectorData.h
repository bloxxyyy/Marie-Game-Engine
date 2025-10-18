#pragma once
#include "ECS/ECSRegistry.h"
#include "ECS/Components.h"
#include <optional>

struct ComponentInspectorData {
    Entity selectedEntity = NULL_ENTITY;

    // We use std::optional because an entity might not have every component.
    std::optional<TagComponent> tag;
    std::optional<TransformComponent> transform;
    std::optional<RenderComponent> render;

};