#ifndef ENTITY_H
#define ENTITY_H
#include "ComponentRegistry.h"

class Entity;
class EntityRef;

class EntityRegistry: public hashedVector<Entity>{
    public:
        EntityRef CreateEntity(ComponentRegistry &componentRegistry);
};

class Entity{
public:

    template <typename DataType>
    std::vector<std::reference_wrapper<DataType>> GetComponents()
    {
        std::vector<std::reference_wrapper<DataType>> components;
        if (!hasComponent<DataType>())
            return components;
        auto& componentIDs = m_components[std::type_index(typeid(DataType))];
        RegistryContainer<DataType>& componentRegistryContainer = m_component_registry->GetContainer<DataType>();
        std::vector<int> removedComponents;
        for (auto &id : componentIDs)
        {
            if (componentRegistryContainer.is_valid_key(id))
                components.push_back(componentRegistryContainer[id]);
            else
                removedComponents.push_back(id);
        }
        for (auto &id : removedComponents)
        {
            componentIDs.erase(std::remove(componentIDs.begin(), componentIDs.end(), id), componentIDs.end());
        }
        return components; 
    }
    
     // Emplace back component to entity
    template <typename DataType, typename... Args>
    void EmplaceComponent(Args&&... args)
    {
        checkValidPointers();
        DataType component(std::forward<Args>(args)...);
        int id = m_component_registry->RegisterComponent(component);
        m_components[std::type_index(typeid(DataType))].push_back(id);
    }

    void Destroy(){
        for (auto& component : m_components)
        {
            if (m_component_registry == nullptr)
                throw std::runtime_error("Component registry reference is null");
            auto container = m_component_registry->FindContainerByTypeIndex(component.first);
            if (container == nullptr)
                throw std::runtime_error("Component container is null");
            for (auto& id : component.second)
            {
                if (container->is_valid_key(id))
                    container->remove(id);
            }
        }
        m_components.clear();
        if (m_entity_registry == nullptr)
            throw std::runtime_error("Entity registry reference is null");
        if(m_entity_registry->is_valid_key(m_id))
            m_entity_registry->remove(m_id);
    }

    template <typename DataType>
    void RemoveComponents()
    {
        checkValidPointers();
        if (!hasComponent<DataType>())
            return;
        auto container = m_component_registry->FindContainerByTypeIndex(std::type_index(typeid(DataType)));
        if (container == nullptr)
            throw std::runtime_error("Component container is null");
        for (const auto& id : m_components[std::type_index(typeid(DataType))])
        {
            if(!container->is_valid_key(id))
                container->remove(id);
        }
        m_components.erase(std::type_index(typeid(DataType)));
    }

    template <typename DataType>
    DataType& AddComponent()
    {
        checkValidPointers();
        DataType component;
        auto componentID = m_component_registry->RegisterComponent<DataType>(component);
        m_components[std::type_index(typeid(DataType))].push_back(componentID);
        return m_component_registry[componentID];
    }

    template <typename DataType>
    bool hasComponent()
    {
        return m_components.find(std::type_index(typeid(DataType))) != m_components.end();
    }

    template <typename DataType>
    void AddComponent(DataType& component)
    {
        checkValidPointers();
        int componentID = m_component_registry->RegisterComponent(component);
        m_components[std::type_index(typeid(DataType))].push_back(componentID);
    }
    //for most purposes it is safer to use an entity reference, be careful when using this
    Entity(const Entity&) = default;
    
private:
friend class EntityRegistry;
    Entity() = default;
    void checkValidPointers()
    {
        if (m_component_registry == nullptr)
            throw std::runtime_error("Component registry reference is null");
        if (m_entity_registry == nullptr)
            throw std::runtime_error("Entity registry reference is null");
    }
    std::unordered_map<std::type_index, std::vector<int>>  m_components;
    EntityRegistry* m_entity_registry = nullptr;
    ComponentRegistry* m_component_registry = nullptr;
    int m_id = -1;
};

class EntityRef{
    public:
        Entity& get(){
            if (m_entityID == -1)
                throw std::runtime_error("Entity ID is -1");
            if (m_entity_registry == nullptr)
                throw std::runtime_error("Entity registry reference is null");
            if (!m_entity_registry->is_valid_key(m_entityID))
                throw std::runtime_error("Entity is not valid");
            return m_entity_registry->operator[](m_entityID);
        }
    int m_entityID = -1;
    private:
    friend class EntityRegistry;
        EntityRegistry* m_entity_registry = nullptr;
};

EntityRef EntityRegistry::CreateEntity(ComponentRegistry & component_registry){
    Entity tempEntity;
    tempEntity.m_entity_registry = this;
    tempEntity.m_component_registry = &component_registry;
    int id = hashedVector<Entity>::push_back(tempEntity);
    Entity &entity = hashedVector<Entity>::operator[](id);
    entity.m_id = id;

    EntityRef ref;
    ref.m_entityID = id;
    ref.m_entity_registry = this;
    return ref;
}

#endif // ENTITY_H

