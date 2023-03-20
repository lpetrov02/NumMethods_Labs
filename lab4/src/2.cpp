#include <algorithm>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <map>
#include <string>


unsigned int numberToGray(unsigned int n) {
    return n ^ (n >> 1);
}


unsigned int grayToNumber(unsigned int g) {
    unsigned int n = 0;
    for (; g!=0; g >>= 1) { n ^= g; }
    return n;
}


struct Result {
    Result(float r, int c, int s) : res(r), code(c), steps(s) {}
    float res;
    int code;
    int steps;
};


struct PtWithVal {
    PtWithVal(float x, float y, unsigned int g) : _x(x), _y(y), gray(g) {}
    PtWithVal(float x, float y) : PtWithVal(x, y, numberToGray(*(unsigned int*)(&x))) {}
    unsigned int gray;
    float _x, _y;
};

bool operator<(PtWithVal& first, PtWithVal& second) {
    return first._y > second._y;
}


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


Result genetic(std::function<float(float)> f, float xMin, float xMax, int populationSize = 10, int maxIter = 10) {
    std::srand(time(NULL));
    std::vector<PtWithVal> population;
    std::vector<PtWithVal> nextPopulation;
    int fCallCnt = 0;
    for (int i = 0; i < populationSize * 2; i++) {
        float x = (float)rand() / (float)RAND_MAX * (xMax - xMin);
        population.push_back(PtWithVal(x, f(x)));
        fCallCnt++;
    }
    for (int i = 0; i < maxIter; i++) {
        nextPopulation.clear();
        // Selection
        for (auto& el : population) {
            nextPopulation.push_back(el);
            std::push_heap(nextPopulation.begin(), nextPopulation.end());
            if (nextPopulation.size() > populationSize) {
                std::pop_heap(nextPopulation.begin(), nextPopulation.end());
                nextPopulation.pop_back();
            }
        }

        population.clear();
        // Crossing & mutations
        for (int i = 0; i < nextPopulation.size(); i++) {
            for (int j = i + 1; j < nextPopulation.size(); j++) {
                unsigned int mask = 1 << (rand() % 32), mask1 = 0, mask2 = 0;
                int bitsToChange;

                unsigned int son1 = (nextPopulation[i].gray & mask) | (nextPopulation[j].gray & (~mask));
                bitsToChange = rand() % 5;
                for (int k = 0; k < bitsToChange; k++) {
                    mask1 |= (1 << rand() % 32);
                }
                son1 ^= mask1;

                unsigned int son2 = (nextPopulation[j].gray & mask) | (nextPopulation[i].gray & (~mask));
                bitsToChange = rand() % 5;
                for (int k = 0; k < bitsToChange; k++) {
                    mask2 |= (1 << rand() % 32);
                }
                son2 ^= mask2;

                unsigned int ui_x1 = grayToNumber(son1), ui_x2 = grayToNumber(son2);
                float x1 = *(float*)(&ui_x1), x2 = *(float*)(&ui_x2);
                population.push_back(PtWithVal(x1, f(x1), son1));
                population.push_back(PtWithVal(x2, f(x2), son2));
                fCallCnt += 2;
            }
        }
    }

    float bestPoint = 0.0, bestVal = -1000000000.0;
    for (auto& el : population) {
        if (el._y > bestVal) {
            bestVal = el._y;
            bestPoint = el._x;
        }
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
    res = genetic(MinusRastrigin, -10.123, 5.234);
    std::cout << "Genetic: " << mapa[res.code] << " " << res.res << "; n_f_calls = " << res.steps << std::endl;

    return 0;
}