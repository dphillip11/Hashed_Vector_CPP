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

    // //check retrieval of components by type
    // auto& p_pos_vector = registry.GetComponentsByType<Particle_position>();
    // test(p_pos_vector.size() == 1); //test11
    // test(p_pos_vector[0].x == 2.5f); //test12
    // auto& float_vector = registry.GetComponentsByType<float>();
    // test(float_vector.size() == 2); //test13

}
