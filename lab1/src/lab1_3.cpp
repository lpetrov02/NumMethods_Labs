#include <bitset>
#include <cmath>
#include <iostream>


float kahan_sum(const float* x, int n) {
    float res = 0, c = 0;
    float y, t;
    for (int i = 0; i < n ; i++) {
        y = x[i] - c;
        t = res + y;
        c = (t - res) - y;
        res = t;
    }
    return res;
}


float not_kahan_sum(const float* x, int n) {
    float res = 0;
    for (int i = 0; i < n; i++) {
        res += x[i];
    }
    return res;
}


void print(float f) {
    std::cout << std::bitset<32>(*reinterpret_cast<uint32_t*>(&f)) << '\n';
}


int main() {
    float* b = new float[100];
    for (int i = 0; i < 100; i++) {
        b[i] = (float)i * (float)i * 5 / 3.f;
    }
    float real_sum = 547250.f;
    std::cout << "Real sum:  ";
    print(real_sum);
    std::cout << "Kahan:     ";
    print(kahan_sum(b, 100));
    std::cout << "Not Kahan: ";
    print(not_kahan_sum(b, 100));

    delete[] b;
    return 0;
}