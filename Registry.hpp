#include "HashedVector.hpp"
#include <typeindex>

namespace Registry
{

    template <typename T>
    class Container
    {
    private:
        friend class Registry;
        HashedVector<T> m_hashedVector;
        std::unordered_map<int, int> m_linkMap;
    };

    class Registry
    {
    private:
        static std::unordered_map<std::type_index, std::string> m_typeNames;

        template <typename T>
        Container<T> &GetContainer()
        {
            //ensures an entry is added to the map, should be useful for file I/O
            m_typeNames[std::type_index(typeid(T))];
            static Container<T> container;
            return container;
        }

    public:
        // get user-friendly type name
        template <typename T>
        std::string GetTypeName()
        {
            auto it = m_typeNames.find(std::type_index(typeid(T)));
            if (it != m_typeNames.end())
            {
                return it->second;
            }
            return "Unknown Type";
        };

        // Function to set the user-friendly type name for a template type
        template <typename T>
        void SetTypeName(const std::string &typeName)
        {
            m_typeNames[std::type_index(typeid(T))] = typeName;
        }
    };
}; // namespace Registry
