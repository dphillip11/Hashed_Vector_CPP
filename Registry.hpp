#ifndef REGISTRY_HPP
#define REGISTRY_HPP
#include "HashedVector.hpp"
#include <typeindex>
#include <memory>

template <typename DataType>
using RegistryContainer = hashedVector<DataType>;

//placeholder class for components
struct Component
{
};

class Registry
{
    friend class Entity;

private:
    static std::unordered_map<std::type_index, std::string> m_typeNames;
    std::unordered_map<std::type_index, std::unique_ptr<RegistryContainer<Component>>> componentContainers;

    template <typename DataType>
    RegistryContainer<DataType> &GetContainer()
    {
        std::type_index typeIndex = std::type_index(typeid(DataType));
        if (componentContainers.count(typeIndex) == 0) {
            componentContainers[typeIndex] = std::make_unique<RegistryContainer<DataType>>();
        }
        return static_cast<RegistryContainer<DataType>&>(*componentContainers[typeIndex]);
    }

        template <typename DataType>
        DataType& GetComponentByComponentID(int componentID)
        {
            return GetContainer<DataType>()[componentID];
        }
    public:
        // get user-friendly type name
        template <typename DataType>
        std::string GetTypeName()
        {
            auto it = m_typeNames.find(std::type_index(typeid(DataType)));
            if (it != m_typeNames.end())
            {
                return it->second;
            }
            return "Unknown Type";
        }

        // Function to set the user-friendly type name for a template type
        template <typename DataType>
        void SetTypeName(const std::string &typeName)
        {
            m_typeNames[std::type_index(typeid(DataType))] = typeName;
        }

        //returns a componentID
        template <typename DataType>
        void RegisterComponent(DataType& component)
        {
            auto& container = GetContainer<DataType>();
            container.push_back(component);
        }

        //returns all components of the given type
        template <typename DataType>
        std::vector<DataType>& GetComponentsByType()
        {
            return GetContainer<DataType>().m_hashedVector.getVector();
        }

        //empties the component container of the given type
        template <typename DataType>
        void DestroyComponents()
        {
            GetContainer<DataType>().clear();
        }
        
        void ClearRegistry(){
            for (auto& container : componentContainers)
            {
                container.second->clear();
            }    
        };
};

class Entity
{
private:
    Registry& m_registry;
    std::unordered_map<std::type_index, std::vector<int>> m_registeredComponents;

public:
    //constructor requires registry reference
    Entity(Registry& registry) : m_registry(registry) {}
    //check if entity has component
    template <typename DataType>
    bool hasComponent()
    {
        return m_registeredComponents.find(std::type_index(typeid(DataType))) != m_registeredComponents.end();
    }
    //removes all components from entity
    void DestroyComponents()
    {
        for (const auto& components : m_registeredComponents)
        {
            for (const auto& id : components.second)
            {
                if(!m_registry.componentContainers[components.first]->is_valid_key(id))
                    continue;
                m_registry.componentContainers[components.first]->remove(id);
            }
        }
        m_registeredComponents.clear();
    }
    //removes components of a given type from entity
    template <typename DataType>
    void RemoveComponents()
    {
        if (!hasComponent<DataType>())
            return;
        for (const auto& id : m_registeredComponents[std::type_index(typeid(DataType))])
        {
            auto& container = m_registry.GetContainer<DataType>();
            if(!container.is_valid_key(id))
                container.remove(id);
        }
        m_registeredComponents.erase(std::type_index(typeid(DataType)));
    }
    //Add component to entity
    template <typename DataType>
    void AddComponent(DataType& component)
    {
        int id = m_registry.RegisterComponent(component);
        m_registeredComponents[std::type_index(typeid(DataType))].push_back(id);
    };
    // returns references to all components of type DataType
    template <typename DataType>
    std::vector<std::reference_wrapper<DataType>> GetComponents()
     {      
            //returns empty vector if entity does not have component
            if (!hasComponent<DataType>())
                return std::vector<std::reference_wrapper<DataType>>();
            auto& componentIDs = m_registeredComponents[std::type_index(typeid(DataType))];
            std::vector<std::reference_wrapper<DataType>> components;
            hashedVector<DataType>& componentHashedVector = m_registry.GetContainer<DataType>().m_hashedVector;
            std::vector<int> removedComponents;
            for (auto &id : componentIDs)
            {
                if (componentHashedVector.tryGet(id))
                    components.push_back(componentHashedVector[id]);
                else
                    removedComponents.push_back(id);
            }
            for (auto &id : removedComponents)
            {
                componentIDs.erase(std::remove(componentIDs.begin(), componentIDs.end(), id), componentIDs.end());
            }
            return components; 
     }
};



#endif // !REGISTRY_HPP
