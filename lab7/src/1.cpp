#include <functional>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>


float abs(float a) {
    return a >= 0 ? a : -a;
}


float sign(float a) {
    return a >= 0 ? 1.0 : -1.0;
}


float integrationNewtonCotes_3(std::function<float(float)> f, std::vector<float> pts) {
    // a = pts[0], b = pts[pts.size() - 1]
    float x1, x2, x3, x4, dx, res = 0;
    for (int i = 0; i < pts.size() - 1; i++) {
        dx = (pts[i + 1] - pts[i]) / 3.f;
        x1 = pts[i], x2 = pts[i] + dx, x3 = pts[i] + dx * 2.f, x4 = pts[i + 1];
        res += dx * 3.f / 8.f * (f(x1) + 3.f * f(x2) + 3.f * f(x3) + f(x4));
    }
    return res;
}


float foo(float x, float h) {
    return exp(-1.f / 2.f * h * h * (1 + x * x)) / (1 + x * x);
}


int main() {
    float a = 18.f, h = 0.1;
    std::vector<float> pts = { 0.f };
    
    float step = a / 9.f;
    for (int i = 0; i < 9; i++) {
        pts.push_back(step * (float)(i + 1));
    }

    float owenResult = integrationNewtonCotes_3([&h](float x){ return foo(x, h); }, pts) / 2.f / M_PI;
    std::cout << "T(" << a << ", " << h << ") = " << owenResult << std::endl;
    return 0;
}