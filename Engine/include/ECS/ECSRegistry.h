#pragma once

#include <any>
#include <cstdint>
#include <map>
#include <memory>
#include <typeindex>
#include <vector>

#include "ISystem.h" 

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;
const Entity NULL_ENTITY = std::numeric_limits<Entity>::max();

class ECSRegistry {
public:
    ECSRegistry();

    void Update(double deltaTime);

    // --- Entity Management ---
    Entity CreateEntity();

    // --- Component Management ---
    template<typename TComponent>
    void AddComponent(Entity entity, TComponent component) {
        GetComponentMap<TComponent>()[entity] = component;
    }

    template<typename TComponent>
    TComponent& GetComponent(Entity entity) {
        // We use .at() to throw an exception if the component doesn't exist.
        return GetComponentMap<TComponent>().at(entity);
    }

    template<typename TComponent>
    bool HasComponent(Entity entity) {
        return GetComponentMap<TComponent>().count(entity);
    }

    template<typename TComponent>
    std::map<Entity, TComponent>& GetComponentMap() {
        std::type_index typeIndex(typeid(TComponent));
        if (m_ComponentMaps.find(typeIndex) == m_ComponentMaps.end()) {
            m_ComponentMaps[typeIndex] = std::map<Entity, TComponent>();
        }
        return std::any_cast<std::map<Entity, TComponent>&>(m_ComponentMaps.at(typeIndex));
    }

    // --- System Management ---
    // A templated helper to easily create and register a system.
    template<typename TSystem, typename... TArgs>
    std::shared_ptr<TSystem> RegisterSystem(TArgs&&... args) {
        auto system = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
        m_Systems.push_back(system);
        return system;
    }

private:
    Entity m_NextEntityID;
    std::map<std::type_index, std::any> m_ComponentMaps;
    std::vector<std::shared_ptr<ISystem>> m_Systems;
};