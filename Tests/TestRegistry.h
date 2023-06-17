#include <iostream>
#include <cassert>
#include <stdexcept>
#include <vector>
#include "Test.h"
#include "../Entity.h"

//declare static members
std::unordered_map<std::type_index, std::string> ComponentRegistry::m_typeNames;

void test_registry()
{
    ComponentRegistry components;
    EntityRegistry entities;

    EntityRef entity1ref = entities.CreateEntity(components);
    EntityRef entity2ref = entities.CreateEntity(components);
    //this option can lead to pointer invalidation so should only be used while the underlying vector does not change
    Entity &entity1 = entity1ref.get();
    //this option is safer but slower
    #define entity2  entity2ref.get()
    char name1 = 'f';
    float health1 = 57.0f;
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
    components.SetTypeName<char>("name");
    components.SetTypeName<float>("health");
    test(components.GetTypeName<char>() == "name"); //test5
    test(components.GetTypeName<float>() == "health"); //test6

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
    auto& CharVector = components.GetComponentsByType<Character>();
    test(CharVector.size() == 1); //test11
    auto& floatVector = components.GetComponentsByType<float>();
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
    components.DestroyComponents<float>();
    test(components.GetComponentsByType<float>().empty()); //test17

    // Test clear components
    entity1.EmplaceComponent<int>(10);
    entity1.EmplaceComponent<int>(20);
    entity1.EmplaceComponent<int>(30);
    entity1.EmplaceComponent<int>(40);
    entity1.EmplaceComponent<int>(50);

    test(entity1.GetComponents<int>().size() == 5); //test18

    components.ClearRegistry();

    test(entity1.GetComponents<int>().empty()); //test19

    // Test typenames are kept after clear
    test(components.GetTypeName<char>() == "name"); //test20
    
    //test entities are kept after reference deleted
    if (true)
    {
        EntityRef temp_entity_ref = entities.CreateEntity(components);
        Entity& temp_entity = temp_entity_ref.get();
        temp_entity.AddComponent(name1);
        temp_entity.AddComponent(health1);
    }
    auto& entitiesVector = entities.getVector();
    auto& stored_entity = entitiesVector.back();
    test(entitiesVector.back().GetComponents<char>()[0] == 'f');    // test21
    test(stored_entity.GetComponents<float>()[0] == 57.0f); //test22
}
