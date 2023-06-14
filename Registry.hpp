#ifndef REGISTRY_HPP
#define REGISTRY_HPP
#include "HashedVector.hpp"
#include <typeindex>



    template <typename DataType>
    class RegistryContainer
    {
    private:
        friend class Registry;
        hashedVector<DataType> m_hashedVector;
        std::unordered_map<int, std::vector<int>> registeredObjectsByEntity;
        std::vector<DataType*> GetComponentsByEntity(int entity)
        {
            std::vector<int> removedObjects;
            std::vector<DataType*> componentVector;
            //check for entity in map
            if(!validEntity(entity))
            {
                //if not found, return empty vector
                return componentVector;
            }           
            //return vector of component references
            for (auto& object: registeredObjectsByEntity[entity])
            {
                if (m_hashedVector.try_get(object))
                    componentVector.push_back(&m_hashedVector[object]);
                    else
                    {
                        removedObjects.push_back(object);
                    }
            }
             registeredObjectsByEntity[entity].erase(std::remove_if(registeredObjectsByEntity[entity].begin(), registeredObjectsByEntity[entity].end(), [&](int element) {
                        return std::find(removedObjects.begin(), removedObjects.end(), element) != removedObjects.end();
                        }),  registeredObjectsByEntity[entity].end());
            return componentVector;
        }

        //-1 will correspond to components not registered with an entity
        void RegisterComponentToEntity(DataType& component, int entity)
        {
            int id = m_hashedVector.emplace_back(component);
            registeredObjectsByEntity[entity].push_back(id);
        }

        bool validEntity(int entity)
        {
            return registeredObjectsByEntity.find(entity) != registeredObjectsByEntity.end();
        }
    
    };

    class Registry
    {
    private:
        static std::unordered_map<std::type_index, std::string> m_typeNames;
 
        template <typename DataType>
        RegistryContainer<DataType> &GetContainer()
        {
            //ensures an entry is added to the map, should be useful for file I/O
            m_typeNames[std::type_index(typeid(DataType))];
            static RegistryContainer<DataType> container;
            return container;
        }
    public:
        template <typename DataType>
        bool entityHasComponent(int entityID)
        {
            return GetContainer<DataType>().validEntity(entityID);
        }
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

        template <typename DataType>
        std::vector<DataType*> GetComponentsByEntity(int entity)
        {
            auto &container = GetContainer<DataType>();
            return container.GetComponentsByEntity(entity);
        }
    
        template <typename DataType>
        void RegisterComponent(DataType& component, int entity = -1)
        {
            auto& container = GetContainer<DataType>();
            container.RegisterComponentToEntity(component, entity);
        }

    };


#endif // !REGISTRY_HPP
