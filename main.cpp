#include "Tests/TestHashedVector.h"
#include "Tests/TestRegistry.h"

//declare static members
std::unordered_map<std::type_index, std::string> ComponentRegistry::m_typeNames;


int main()
{
    test_registry();
    std::cout <<"end of registry test" << std::endl;
    testHashedVector();
    std::cout <<"end of hashed vector test" << std::endl;
    return 0;
}
