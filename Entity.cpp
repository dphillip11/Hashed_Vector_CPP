#include "Entity.h"



EntityRegistry::EntityRegistry(ComponentRegistry &componentRegistry): m_component_registry(&componentRegistry) {}

Entity EntityRegistry::CreateEntity(){
    Entity entity;
    entity.m_id = m_nextID++;
    entity.m_registry = this;
    return entity;
}

//removes all components from component registry and removes entity from entity registry
void EntityRegistry::DestroyEntity(const EntityID& entityID){
    //remove all components from component registry
    auto& all_components = m_entity_components[entityID];
    for(auto& pair : all_components){
        auto components = m_component_registry->FindContainerByTypeIndex(pair.first);
        for(auto& componentID : pair.second){
            components->remove(componentID);
        }
    }
    //remove entity from entity registry
    m_entity_components.erase(entityID);
}

void EntityRegistry::DestroyEntity(Entity& entity){
    DestroyEntity(entity.m_id);
}

Entity EntityRegistry::GetEntity(int id){
    Entity entity;
    entity.m_id = id;
    entity.m_registry = this;
    return entity;
}

int EntityRegistry::GetEntityCount(){
    return m_entity_components.size();
}

//only clears entity registry, not component registry
void EntityRegistry::Clear(){
    m_entity_components.clear();
}

//removes all entities from entity registry and removes all related components from component registry
//components not registered to an entity are not removed
void EntityRegistry::DestroyAllEntities()
{
    for (auto& each : m_entity_components)
    {
        DestroyEntity(each.first);
    }
}

