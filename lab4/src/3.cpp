#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


int main() {
    float x = 999999999999.1;
    unsigned int y = *(unsigned int*)(&x);
    float z = *(float*)(&y);
    std::cout << y << std::endl;
    std::cout << z << std::endl;
    return 0;
}