#pragma once
#include <unordered_map>
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
class RegistryContainer : public BaseRegistryContainer, public hashedVector<DataType>
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

class ComponentRegistry
{
private:
    friend class Entity;
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

    BaseRegistryContainer* FindContainerByTypeIndex(std::type_index typeIndex)
    {
        if (componentContainers.count(typeIndex) == 0) {
            return nullptr;
        }
        return (&(*componentContainers[typeIndex]));
    }
public:

    template <typename DataType>
    DataType& GetComponentByComponentID(int componentID)
    {
        auto& container = GetContainer<DataType>();
        if (!container.is_valid_key(componentID))
            throw std::runtime_error("Component ID is invalid"); 
        return GetContainer<DataType>()[componentID];
    }

    template <typename DataType>
    bool HasComponent(int componentID)
    {
        return GetContainer<DataType>().is_valid_key(componentID);
    }
    
    template <typename DataType>
    void RemoveComponentByID(int componentID)
    {
        GetContainer<DataType>().remove(componentID);
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


