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


Result lipo(std::function<float(float)> f, float xMin, float xMax, int maxPoints = 100, int maxIter = 100) {
    std::srand(time(NULL));
    std::vector<float> pts = { xMin, xMax };
    std::map<float, float> vals = { { xMin, f(xMin) }, { xMax, f(xMax) } };  // мб это не нужно, если там всё закэшируется
    float L = abs(f(xMax) - f(xMin)) / abs(xMax - xMin), x = 0.0, upperBound = 0.0;
    float inf = -1000000000;
    int leftNeighbourInd = 0;
    float bestPoint = vals[xMin] < vals[xMax] ? xMax : xMin;
    int fCallCnt = 2;
    while (pts.size() < maxPoints) {
        bool goodPointFound = false;
        for (int i = 0; i < maxIter; i++) {
            leftNeighbourInd = rand() % (pts.size() - 1);
            x = (pts[leftNeighbourInd] + pts[leftNeighbourInd + 1]) / 2.0 + (vals[pts[leftNeighbourInd + 1]] - vals[pts[leftNeighbourInd]]) / 2.0 / L;
            upperBound = vals[pts[leftNeighbourInd]] + L * abs(x - pts[leftNeighbourInd]);
            if (upperBound > vals[bestPoint]) {
                break;
            }
        }
        if (upperBound <= vals[bestPoint]) {
            return Result(bestPoint, 1, fCallCnt);
        }
        pts.push_back(x);
        vals[x] = f(x);
        fCallCnt++;
        bestPoint = vals[x] > vals[bestPoint] ? x : bestPoint;
        for (int i = leftNeighbourInd; i <= leftNeighbourInd + 1; i++) {
            L = std::max(L, abs(vals[x] - vals[pts[i]]) / abs(x - pts[i]));
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
    res = lipo(MinusRastrigin, -10.123, 5.234);
    std::cout << "LIPO: " << mapa[res.code] << " " << res.res << "; n_f_calls = " << res.steps << std::endl;

    return 0;
}