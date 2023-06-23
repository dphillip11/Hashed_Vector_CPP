#include <iostream>
#include <cassert>
#include <stdexcept>
#include <vector>
#include "Test.h"
#include "../Entity.h"

void test_registry()
{
    ComponentRegistry components;
    EntityRegistry entities(components);

    auto entity1 = entities.CreateEntity();
    auto entity2 = entities.CreateEntity();
    char name1 = 'f';
    float health1 = 57.0f;
    char name2 = 'n';
    float health2 = 99.0f;

    entity1.AddComponent(name1);
    entity1.AddComponent(health1);
    entity2.AddComponent(name2);
    entity2.AddComponent(health2);
    //check retrieval of elements by entity
    test(entity1.GetComponent<char>(0) == 'f'); //test1
    test(*entity2.GetComponents<char>()[0] == 'n'); //test2
    test(entity1.GetComponent<float>(0) == 57.0f); //test3
    test(*entity2.GetComponents<float>()[0] == 99.0f); //test4

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
    test(entity1.GetComponent<Character>(0).character == Char.character); //test7
    test(entity1.GetComponents<FloatingPoint>()[0]->floating == Float.floating); //test8
    test(entity1.GetComponents<Character>().size() == 1); //test9
    test(entity1.GetComponents<FloatingPoint>().size() == 1); //test10

    //check retrieval of components by type
    auto& CharVector = components.GetContainer<Character>().getVector();
    test(CharVector.size() == 1); //test11
    auto& floatVector = components.GetContainer<float>().getVector();
    test(floatVector.size() == 2); //test12

     // Test emplace component
    entity1.AddComponent<int>(10);
    test(*entity1.GetComponents<int>()[0] == 10); //test13

    // Test hasComponent
    test(entity1.HasComponent<int>()); //test14
    test(!entity1.HasComponent<double>()); //test15

    // Test remove components
    entity1.RemoveComponents<int>();
    test(!entity1.HasComponent<int>()); //test16

    // Test destroy components
    components.GetContainer<float>().clear();
    test(components.GetContainer<float>().size() == 0); //test17

    // Test clear components
    entity1.AddComponent<int>(10);
    entity1.AddComponent<int>(20);
    entity1.AddComponent<int>(30);
    entity1.AddComponent<int>(40);
    entity1.AddComponent<int>(50);

    test(entity1.GetComponents<int>().size() == 5); //test18

    components.ClearRegistry();

    test(entity1.GetComponents<int>().empty()); //test19

    // Test typenames are kept after clear
    test(components.GetTypeName<char>() == "name"); //test20

    int id;
    // test entities are kept after reference deleted
    if (true)
    {
        Entity temp_entity = entities.CreateEntity();
        temp_entity.AddComponent(name1);
        temp_entity.AddComponent(health1);
        id = temp_entity.m_id;
    }
    auto stored_entity = entities.GetEntity(id);
    test(*stored_entity.GetComponents<char>()[0] == 'f');    // test21
    test(*stored_entity.GetComponents<float>()[0] == 57.0f); //test22
}
