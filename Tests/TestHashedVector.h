
#include <stdexcept>
#include <vector>
#include "../HashedVector.hpp"
#include <string>
#include "Test.h"

// Test function to check the functionality of HashedVector
void testHashedVector()
{
    hashedVector<char> hv;
    //test adding elements
    test(hv.push_back('A') == 0); // test 1
    test(hv.push_back('B') == 1); //test2
    test(hv.push_back('C') == 2); //test3
    test(hv.size() == 3); //test4
    //test retrieval
    test(hv[0] == 'A'); //test5
    test(hv[1] == 'B'); //test6
    test(hv[2] == 'C'); //test7
    //test removing elements
    hv.remove(1);
    test(hv.size() == 2); // test8
    // test accuracy of elements
    test(hv[0] == 'A'); //test9
    test(hv[2] == 'C'); //test10
    //test emplacement
    auto id = hv.emplace_back('D');
    test(hv.size() == 3); //test11
    test(hv[id] == 'D'); //test12

    struct testStruct
    {
        int a;
        float b;
        testStruct(int a, float b) : a(a), b(b) {}
    };
    hashedVector<testStruct> hv2;
    auto id2 = hv2.emplace_back(1, 2.0f);
    test(hv2[id2].a == 1); //test13
    test(hv2[id2].b == 2.0f); //test14
    test(hv2.size() == 1); //test15
    hv2.remove(id2);
    test(hv2.size() == 0); //test16
    //test removing all elements
    hv2.clear();
    //check ID's are not reused
    test(hv2.emplace_back(0, 1.0f) == 1);//test17
    test(hv2.emplace_back(0, 1.0f) == 2); //test18

    //test lookup accuracy over a large number of element
    hashedVector<int> hv3;
    for(int i = 0; i < 10000; i++)
    {
        hv3.emplace_back(i);
    }
    bool success = true;
    for(int i = 0; i < 10000; i++)
    {
        //check random access
        int index = rand() % 10000;
        if(hv3[index] != index)
        {
            success = false;
            break;
        }
    }
    test(success); //test19
    //remove non sequential elements
    for (int i = 0; i < 1000; i++)
    {
        int index = i *  5;
        hv3.remove(index);
    }
    test(hv3.size() == 9000); //test20
    //check hash map size
    test(hv3.getHashMap().size() == 9000);//test21
    // check accuracy of hash map
    for (int i = 0; i < 10000; i++)
    {
        if (!hv3.is_valid_key(i))
        {
            continue;
        }
        if (hv3[i] != i)
        {
            success = false;
            std::cout << "diff: " << i - hv3[i] << std::endl;
            break;
        }
        }
    test(success); //test22
    //check valid lookup by value
    hashedVector<int> hv4;
    hv4.emplace_back(1);
    hv4.emplace_back(2);
    int id3 = hv4.emplace_back(3);
    hv4.emplace_back(4);
    hv4.emplace_back(5);
    int three = 3;
    test(hv4.get_id(three) == id3); //test23
    //test remove by value
    hv4.remove(three);
    test(hv4.size() == 4); //test24
}
