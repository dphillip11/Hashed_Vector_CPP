#include <iostream>
#include <cassert>
#include <stdexcept>
#include <vector>
#include "../Registry.hpp"

//declare static members
std::unordered_map<std::type_index, std::string> Registry::m_typeNames;

void test_registry()
{
    Registry registry;
    auto& container = registry.GetContainer<char>();
    std::cout << "got container" << std::endl;
    int Entity57 = 57;
    char name1 = 'f';
    float health1 = 57.0f;
    int Entity99 = 99;
    char name2 = 'n';
    float health2 = 99.0f;

    registry.RegisterComponent(name1, Entity57);
    registry.RegisterComponent(health1, Entity57);
    registry.RegisterComponent(name2, Entity99);
    registry.RegisterComponent(health2, Entity99);
    registry.SetTypeName<char>("name");
    registry.SetTypeName<float>("health");
    std::cout <<registry.GetTypeName<char>()<< std::endl;
    std::cout <<registry.GetTypeName<float>()<< std::endl;
    auto retrieved_name = registry.GetComponentsByEntity<char>(Entity57);
    std::cout << *retrieved_name[0] << std::endl;
}
