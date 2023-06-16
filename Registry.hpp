#ifndef REGISTRY_HPP
#define REGISTRY_HPP
#include "HashedVector.hpp"
#include <typeindex>
#include <memory>

class BaseRegistryContainer {
public:
    virtual ~BaseRegistryContainer() = default;
    virtual void clear() = 0;
    virtual bool is_valid_key(int id) const = 0;
    virtual void remove(int id) = 0;
    virtual void reserve(const int &size) = 0;
};

template <typename DataType>
class RegistryContainer: public BaseRegistryContainer, public hashedVector<DataType>
{
public:
    RegistryContainer() = default;
    ~RegistryContainer() = default;
    void clear() override
    {
        hashedVector<DataType>::clear();
    }
    bool is_valid_key(int id) const override
    {
        return hashedVector<DataType>::is_valid_key(id);
    }
    void remove(int id) override
    {
        hashedVector<DataType>::remove(id);
    }
    void reserve(const int &size) override
    {
        hashedVector<DataType>::reserve(size);
    }
};

class Registry
{
    friend class Entity;

private:
    static std::unordered_map<std::type_index, std::string> m_typeNames;
    std::unordered_map<std::type_index, std::unique_ptr<BaseRegistryContainer>> componentContainers;

    template <typename DataType>
    RegistryContainer<DataType> &GetContainer()
    {
        std::type_index typeIndex = std::type_index(typeid(DataType));
        if (componentContainers.count(typeIndex) == 0) {
            componentContainers[typeIndex] = std::make_unique<RegistryContainer<DataType>>();
        }
        return static_cast<RegistryContainer<DataType>&>(*componentContainers[typeIndex]);
    }

public:
    template <typename DataType>
    DataType& GetComponentByComponentID(int componentID)
    {
        return GetContainer<DataType>()[componentID];
    }

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

    template <typename DataType>
    void SetTypeName(const std::string &typeName)
    {
        m_typeNames[std::type_index(typeid(DataType))] = typeName;
    }

    template <typename DataType>
    int RegisterComponent(DataType& component)
    {
        auto& container = GetContainer<DataType>();
        return container.push_back(component);
    }

    template <typename DataType>
    std::vector<DataType>& GetComponentsByType()
    {
        return GetContainer<DataType>().getVector();
    }

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
    Entity(Registry& registry) : m_registry(registry) {}

    template <typename DataType>
    bool hasComponent()
    {
        return m_registeredComponents.find(std::type_index(typeid(DataType))) != m_registeredComponents.end();
    }

    void DestroyComponents()
    {
        for (const auto& components : m_registeredComponents)
        {
            for (const auto& id : components.second)
            {   
                if(m_registry.componentContainers.find(components.first) == m_registry.componentContainers.end())
                    continue;
                auto& container = m_registry.componentContainers[components.first];
                if(!container->is_valid_key(id))
                    continue;
                container->remove(id);
            }
        }
        m_registeredComponents.clear();
    }

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

    template <typename DataType>
    void AddComponent(DataType& component)
    {
        int componentID = m_registry.RegisterComponent(component);
        m_registeredComponents[std::type_index(typeid(DataType))].push_back(componentID);
    }

    // Emplace back component to entity
    template <typename DataType, typename... Args>
    void EmplaceComponent(Args&&... args)
    {
        DataType component(std::forward<Args>(args)...);
        int id = m_registry.RegisterComponent(component);
        m_registeredComponents[std::type_index(typeid(DataType))].push_back(id);
    }
    
    template <typename DataType>
    std::vector<std::reference_wrapper<DataType>> GetComponents()
    {
        if (!hasComponent<DataType>())
            return std::vector<std::reference_wrapper<DataType>>();
        auto& componentIDs = m_registeredComponents[std::type_index(typeid(DataType))];
        std::vector<std::reference_wrapper<DataType>> components;
        RegistryContainer<DataType>& componentRegistryContainer = m_registry.GetContainer<DataType>();
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
};

#endif // !REGISTRY_HPP
