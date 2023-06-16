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
    
    Entity entity1(registry);
    char name1 = 'f';
    float health1 = 57.0f;
    Entity entity2(registry);
    char name2 = 'n';
    float health2 = 99.0f;

    entity1.AddComponent(name1);
    entity1.AddComponent(health1);
    entity2.AddComponent(name2);
    entity2.AddComponent(health2);
    
    //check retrieval of elements by entity
    test(entity1.GetComponents<char>()[0] == 'f'); //test1
    test(entity2.GetComponents<char>()[0] == 'n'); //test2
    test(entity1.GetComponents<float>()[0] == 57.0f); //test3
    test(entity2.GetComponents<float>()[0] == 99.0f); //test4

    //check storage of user friendly type names
    registry.SetTypeName<char>("name");
    registry.SetTypeName<float>("health");
    test(registry.GetTypeName<char>() == "name"); //test5
    test(registry.GetTypeName<float>() == "health"); //test6

    //test custom components are differentiated by type
    struct Character
    {
        char character;
    };
    struct FloatingPoint
    {
        float floating;
    };

    Character Char = {'C'};
    FloatingPoint Float = { 4.0f };
    entity1.AddComponent(Char);
    entity1.AddComponent(Float);

    //check retrieval of elements by entity
    test(entity1.GetComponents<Character>()[0].get().character == Char.character); //test7
    test(entity1.GetComponents<FloatingPoint>()[0].get().floating == Float.floating); //test8
    test(entity1.GetComponents<Character>().size() == 1); //test9
    test(entity1.GetComponents<FloatingPoint>().size() == 1); //test10

    //check retrieval of components by type
    auto& CharVector = registry.GetComponentsByType<Character>();
    test(CharVector.size() == 1); //test11
    auto& floatVector = registry.GetComponentsByType<float>();
    test(floatVector.size() == 2); //test12

     // Test emplace component
    entity1.EmplaceComponent<int>(10);
    test(entity1.GetComponents<int>()[0] == 10); //test13

    // Test hasComponent
    test(entity1.hasComponent<int>()); //test14
    test(!entity1.hasComponent<double>()); //test15

    // Test remove components
    entity1.RemoveComponents<int>();
    test(!entity1.hasComponent<int>()); //test16

    // Test destroy components
    registry.DestroyComponents<float>();
    test(registry.GetComponentsByType<float>().empty()); //test17

    // Test clear registry
    entity1.EmplaceComponent<int>(10);
    entity1.EmplaceComponent<int>(20);
    entity1.EmplaceComponent<int>(30);
    entity1.EmplaceComponent<int>(40);
    entity1.EmplaceComponent<int>(50);

    test(entity1.GetComponents<int>().size() == 5); //test18

    registry.ClearRegistry();

    test(entity1.GetComponents<int>().empty()); //test19

    
}
