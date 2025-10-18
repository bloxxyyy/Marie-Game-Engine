#pragma once

#include "ECS/ECSRegistry.h"
#include <vector>
#include <string>

struct EntityInfo {
    Entity id;
    std::string tag;
};

struct EntityListData {
    std::vector<EntityInfo> entities;
    Entity selectedEntity = NULL_ENTITY;
};