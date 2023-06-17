#pragma once

#include <iostream>

// Test function to check if the given condition is true
void test(bool condition)
{
    static int testNumber = 1;
    std::cout << "Test " << testNumber << ": " << (condition ? "Pass" : "Fail") << std::endl;
    testNumber++;
}
