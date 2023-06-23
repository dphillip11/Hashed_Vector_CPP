#ifndef ENTITY_H
#define ENTITY_H
#include "ComponentRegistry.h"

class Entity;
using EntityID = int;
using ComponentID = int;

class EntityRegistry{
    private:
        friend class Entity;
        int m_nextID = 0;
        ComponentRegistry *m_component_registry;
        std::unordered_map<EntityID, std::unordered_map<std::type_index, std::vector<ComponentID>>> m_entity_components;
    public:
        EntityRegistry(ComponentRegistry &componentRegistry);
        Entity CreateEntity();
        void DestroyEntity(Entity& entity);
        void DestroyEntity(const EntityID& entityID);
        void DestroyAllEntities();
        Entity GetEntity(int id);
        int GetEntityCount();
        void Clear();
};

class Entity{
public:
    EntityID m_id = -1;

    //assumes vector index is valid
    template <typename ComponentType>
    ComponentType& GetComponent(int vectorIndex)
    {
        auto& components = GetComponentIDs<ComponentType>();
        auto& container = m_registry->m_component_registry->GetContainer<ComponentType>();
        return container[components[vectorIndex]];
    }

    //returns a vector of references to components
    template <typename ComponentType>
    std::vector<ComponentType*> GetComponents()
    {
        std::vector<ComponentType*> components;
        auto& componentIDs = GetComponentIDs<ComponentType>();
        auto& container = m_registry->m_component_registry->GetContainer<ComponentType>();
        for(auto& componentID : componentIDs){
            if (container.is_valid_key(componentID))
                components.push_back(&container[componentID]);
            else
                componentIDs.erase(std::remove(componentIDs.begin(), componentIDs.end(), componentID), componentIDs.end());
        }
        return components;
    }

    //check if component vector is empty
    template <typename ComponentType>
    bool HasComponent()
    {
        auto& components = GetComponentIDs<ComponentType>();
        return components.size() > 0;
    }

    //copies component and returns an updated reference to the component
    template <typename ComponentType>
    auto& AddComponent(const ComponentType& component){
        auto& components = GetComponentIDs<ComponentType>();
        auto& container = m_registry->m_component_registry->GetContainer<ComponentType>();
        auto componentID = container.push_back(component);
        components.push_back(componentID);
        return container[componentID];
    }

    //emplaces component created with given arguments and returns a reference to the component
    template <typename ComponentType, typename... Args>
    auto& NewComponent(Args&&... args){
        auto& components = GetComponentIDs<ComponentType>();
        auto& container = m_registry->m_component_registry->GetContainer<ComponentType>();
        auto componentID = container.emplace_back(std::forward<Args>(args)...);
        components.push_back(componentID);
        return container[componentID];
    }

    //removes component from component registry and removes component from entity registry
    template <typename ComponentType>
    void RemoveComponent(ComponentType& component){
        auto& components = GetComponentIDs<ComponentType>();
        auto& container = m_registry->m_component_registry->GetContainer<ComponentType>();
        auto componentID = container.get_id(component);
        container.remove(componentID);
        components.erase(std::remove(components.begin(), components.end(), componentID), components.end());
    }

    template <typename ComponentType>
    void RemoveComponents(){
        auto& components = GetComponentIDs<ComponentType>();
        auto& container = m_registry->m_component_registry->GetContainer<ComponentType>();
        for(auto& componentID : components){
            container.remove(componentID);
        }
        components.clear();
    }

    void Destroy(){
        m_registry->DestroyEntity(m_id);
    }

private:
    template <typename ComponentType>
    std::vector<ComponentID>& GetComponentIDs(){
        return m_registry->m_entity_components[m_id][std::type_index(typeid(ComponentType))];
    }

    friend class EntityRegistry;
    EntityRegistry* m_registry = nullptr;
};

#endif // ENTITY_H

