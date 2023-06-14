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
    
    //check retrieval of elements by entity
    test(*registry.GetComponentsByEntity<char>(Entity57)[0] == 'f'); //test1
    test(*registry.GetComponentsByEntity<char>(Entity99)[0] == 'n'); //test2
    test(*registry.GetComponentsByEntity<float>(Entity57)[0] == 57.0f); //test3
    test(*registry.GetComponentsByEntity<float>(Entity99)[0] == 99.0f); //test4

    //check storage of user friendly type names
    registry.SetTypeName<char>("name");
    registry.SetTypeName<float>("health");
    test(registry.GetTypeName<char>() == "name"); //test5
    test(registry.GetTypeName<float>() == "health"); //test6


}
