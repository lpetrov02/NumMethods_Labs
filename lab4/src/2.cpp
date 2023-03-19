#include <functional>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <map>
#include <string>


struct Result {
    Result(float r, int c, int s) : res(r), code(c), steps(s) {}
    float res;
    int code;
    int steps;
};


float abs(float a) {
    return a >= 0 ? a : -a;
}


Result randomSearch(std::function<float(float)> f, float xMin, float xMax, int maxIter = 1000) {
    std::srand(time(NULL));
    float inf = -1000000000, currentMax = std::max(f(xMin), f(xMax));
    int fCallCnt = 2;
    float x = 0.0, bestPoint = f(xMin) > f(xMax) ? xMin : xMax;
    for (int i = 0; i < maxIter; i++) {
        x = xMin + (float)(rand()) / (float)(RAND_MAX) * (xMax - xMin);
        if (f(x) > currentMax) {
            currentMax = f(x);
            bestPoint = x;
        }
        fCallCnt++;
    }
    return Result(bestPoint, 0, fCallCnt);
}


Result genetic(std::function<float(float)> f, float xMin, float xMax, float primFreq = 5, int maxPoints = 100, int maxIter = 100) {
    std::vector<float> population;
    float eps = 
    for (float x = xMin; x <= xMax; x += primFreq) {
        population.push_back(x - x);
    }
    return Result(bestPoint, 0, fCallCnt);
}


float MinusRastrigin(float x) {
    return -10.0 - x * x + 10.0 * cos(2.0 * M_PI * x);
}


int main() {
    std::map<int, std::string> mapa = { {0, "SUCCESS"}, {1, "NO GOOD POINTS ANYMORE"} };

    std::cout << "Rastrigin's function" << std::endl;
    auto res = randomSearch(MinusRastrigin, -10.123, 5.234);
    std::cout << "RandomSearch: " << mapa[res.code] << " " << res.res << "; n_f_calls = " << res.steps << std::endl;
    res = lipo(MinusRastrigin, -10.123, 5.234);
    std::cout << "LIPO: " << mapa[res.code] << " " << res.res << "; n_f_calls = " << res.steps << std::endl;
}