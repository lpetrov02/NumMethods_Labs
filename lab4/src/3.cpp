#include <cmath>
#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <string>
#include <sstream>


float abs(float x) {
    return x >= 0 ? x : -x;
}


std::vector<float> solve(std::function<float(float)> f, float xMin, float xMax, float L, int maxIter = 100) {
    float step = 1.0, eps = 0.001, eps1 = 0.001, eps2 = 0.001;
    std::vector<float> pts;
    for (float p = xMin; p < xMax - eps; p += step) {
        pts.push_back(p);
    }
    pts.push_back(xMax);

    std::vector<float> answers;
    for (int i = 0; i < pts.size() - 1; i++) {
        if (abs(f(pts[i])) - L * abs(pts[i] - pts[i + 1]) > 0) {
            continue;
        }
        if (f(pts[i]) * f(pts[i + 1]) > 0) {
            continue;
        }

        float x1 = pts[i], x2 = pts[i + 1], dx = 0.0, dy = 0.0;
        int iterCnt = 0;
        do {
            float mid = (x1 + x2) / 2.0;
            if (f(mid) * f (x1) < 0) {
                dx = abs(x2 - mid);
                dy = abs(f(x2) - f(mid));
                x2 = mid;
            }
            else {
                dx = abs(1 - mid);
                dy = abs(f(x1) - f(mid));
                x1 = mid;
            }
            iterCnt++;
        } while (iterCnt < maxIter && (dx > eps1 || dy > eps2));
        answers.push_back(x1);
    }
    return answers;
}


float f(float x) {
    return sin(x);
}


int main() {
    auto answer = solve(f, (float)-20.4, (float)20.6, 1.f);
    for (auto& ans : answer) {
        std::cout << ans << std::endl;
    }
    return 0;
}