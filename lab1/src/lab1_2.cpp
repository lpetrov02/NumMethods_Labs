#include <bitset>
#include <cmath>
#include <iostream>


float polynomial(float x, const float* a, int n) {
    float res = 0;
    for (int i = n - 1; i >= 0; i--) {
        res = fma(res, x, a[i]);
    }
    return res;
}


void print(float f) {
    std::cout << std::bitset<32>(*reinterpret_cast<uint32_t*>(&f)) << '\n';
}


int main() {
    float* a = new float[10];
    for (int i = 0; i < 10; i++) {
        a[i] = i + 1;
    }
    std::cout << "Ploynomial: " << polynomial(2, a, 10) << std::endl;

    delete[] a;
    return 0;
}