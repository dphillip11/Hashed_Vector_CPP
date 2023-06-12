#include <iostream>
#include <vector>
#include "HashedVector.hpp"
#include <cassert>

void testHashedVector()
{
  bool testPassed = true;

    HashedVector<int> hashedVector;

    // Add elements to the HashedVector
    int id1 = hashedVector.push_back(10);
    int id2 = hashedVector.push_back(20);
    int id3 = hashedVector.push_back(30);

    // Modify and access elements using the [] operator
    hashedVector[id1] = 100;
    hashedVector[id2] = 200;
    hashedVector[id3] = 300;

    // Access elements using try_get and print their values
    if (int* element1 = hashedVector.try_get(id1)) {
        std::cout << "Element 1: " << *element1 << std::endl;
    } else {
        testPassed = false;
        std::cout << "Failed to access Element 1 using try_get." << std::endl;
    }
    if (int* element2 = hashedVector.try_get(id2)) {
        std::cout << "Element 2: " << *element2 << std::endl;
    } else {
        testPassed = false;
        std::cout << "Failed to access Element 2 using try_get." << std::endl;
    }
    if (int* element3 = hashedVector.try_get(id3)) {
        std::cout << "Element 3: " << *element3 << std::endl;
    } else {
        testPassed = false;
        std::cout << "Failed to access Element 3 using try_get." << std::endl;
    }

    // Remove an element from the HashedVector
    hashedVector.remove(id2);

    // Try accessing the removed element, should return nullptr
    if (int* removedElement = hashedVector.try_get(id2)) {
        testPassed = false;
        std::cout << "Element 2 still exists after removal. Test failed." << std::endl;
    }

    // Try accessing an invalid index, should throw an exception
    try {
        int invalidElement = hashedVector[999];
        testPassed = false;
        std::cout << "Invalid Element accessed without throwing an exception. Test failed." << std::endl;
    } catch (const std::out_of_range&) {
        // Exception caught as expected
    }

    // Check pass or fail condition
    if (testPassed) {
        std::cout << "All test cases passed!" << std::endl;
    } else {
        std::cout << "One or more test cases failed." << std::endl;
    }

     //keep a list of floats and their pointerss
	std::vector<float*> std_vector_pointers;
	std::vector<float> vector;
	HashedVector<float> hashed_vector;

	for (int i = 0; i < 10; i++)
	{
		vector.push_back(i * 1.0f);
		std_vector_pointers.push_back(&vector.back());
        auto id = hashed_vector.push_back(i * 1.0f);
        assert(id == i);
	}

    auto Test = [](std::vector<float*>& std_vector_pointers, HashedVector<float>& hashed_vector){
        int valid_vector_points = 0;
        int valid_hashed_vector_points = 0;
        //demonstrate pointers are invalid
        for (int i = 0; i < 10; i++)
        {
            auto vector_success = *std_vector_pointers[i] == i * 1.0f;
            auto hashed_vector_success = hashed_vector[i] == i * 1.0f;
            valid_vector_points += vector_success;
            valid_hashed_vector_points += hashed_vector_success;
        }
        std::cout << "Vector: " << (valid_vector_points) << std::endl;
        std::cout << "HashedVector: " << (valid_hashed_vector_points) << std::endl;
    };

	Test(std_vector_pointers, hashed_vector);

    //remove 5 elements
    for (int i = 0; i < 5; i++)
    {
        vector.erase(vector.begin() + i);
        hashed_vector.remove(i);
    }
    //add 5 elements
    for (int i = 0; i < 5; i++)
    {
        vector.push_back(i * 1.0f);
        auto id = hashed_vector.push_back(i * 1.0f);
        assert(id == i + 10);
    }
    //now the data should be 5678901234
    //valid id's should be 5-14
    int expected[] = {5,6,7,8,9,0,1,2,3,4};
    int valid_vector_points = 0;
    int valid_hashed_vector_points = 0;

    for (int i = 0; i < 10; i++)
    {
        auto vector_success = *std_vector_pointers[i] == expected[i] * 1.0f;
        auto hashed_vector_success = hashed_vector[i + 5] == expected[i] * 1.0f;
        valid_hashed_vector_points += hashed_vector_success;
        valid_vector_points += vector_success;
    }
    std::cout << "Vector: " << (valid_vector_points) << std::endl;
    std::cout << "HashedVector: " << (valid_hashed_vector_points) << std::endl;
}

int main(){
    testHashedVector();
    return 0;

}
