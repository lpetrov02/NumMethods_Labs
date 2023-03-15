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


Result bisection(std::function<float(float)> f, int maxSteps = 10000, float eps1 = 0.001, float eps2 = 0.001) {
    // f = F' if we want to optimize F
    std::srand(time(NULL));
    float x1 = -10.0, x2 = 10.0;
    int maxSteps0 = 100;
    int stepsCnt = 0;
    while (f(x1) >= 0 && stepsCnt < maxSteps0) {
        x1 -= (float)(std::rand() % 1000) / 10;
        stepsCnt++;
    }
    while (f(x2) <= 0 && stepsCnt < maxSteps0) {
        x2 += (float)(std::rand() % 1000) / 10;
        stepsCnt++;
    }
    if (f(x1) * f(x2) >= 0) {
        return Result(0, 2, -1);
    }

    float dx = 0.0, dy = 0.0;
    stepsCnt = 0;
    do {
        float mid = (x1 + x2) / 2;
        if (f(x1) * f(mid) < 0) {
            dx = x2 - mid;
            dy = abs(f(x2) - f(mid));
            x2 = mid;
        }
        else {
            dx = mid - x1;
            dy = abs(f(x1) - f(mid));
            x1 = mid;
        }
        stepsCnt++;

    } while (stepsCnt < maxSteps && (dx > eps1 || dy > eps2));

    if (dx > eps1 || dy > eps2) {
        return Result(x1, 1, stepsCnt);
    }
    return Result(x1, 0, stepsCnt);
}


Result newton(std::function<float(float)> f, std::function<float(float)> der1, std::function<float(float)> der2, int maxSteps = 10000, float eps1 = 0.001, float eps2 = 0.001, float x = 0.0) {
    std::srand(time(NULL));
    int stepsCnt = 0;

    float dx = 0.0, dy = 0.0;
    stepsCnt = 0;
    do {
        float x1 = x - der1(x) / der2(x);
        dx = abs(x - x1);
        dy = abs(f(x) - f(x1));
        x = x1;
        stepsCnt++;

    } while (stepsCnt < maxSteps && (dx > eps1 || dy > eps2));

    if (dx > eps1 || dy > eps2) {
        return Result(x, 1, stepsCnt);
    }
    return Result(x, 0, stepsCnt);
}


float foo(float x) {
    return 10.0 + x * x - 10.0 * cos(2.0 * M_PI * x);
}

float fooDer(float x) {
    return 2.0 * x + 20.0 * M_PI * sin(2.0 * M_PI * x);
}

float fooDer2(float x) {
    return 40.0 * M_PI * M_PI * cos(20.0 * M_PI * x);
}

float parabola(float x) {
    return 5.0 * x * x - 4.0 * x + 3.0;
}

float parabolaDer(float x) {
    return 10.0 * x - 4.0;
}

float parabolaDer2(float x) {
    return 10.0;
}


int main() {
    std::map<int, std::string> mapa = { {0, "SUCCESS"}, {1, "ITER LIMIT EXCEEDED"}, {2, "INIT FAILURE"} };

    std::cout << "Rastrigin's function" << std::endl;
    auto res1 = bisection(fooDer);
    std::cout << "Bisection: " << mapa[res1.code] << " " << res1.res << "; n_steps = " << res1.steps << std::endl;
    res1 = newton(foo, fooDer, fooDer2, 5.0);
    std::cout << "Newton: " << mapa[res1.code] << " " << res1.res << "; n_steps = " << res1.steps << std::endl;

    auto res2 = bisection(parabolaDer);
    std::cout << "Bisection: " << mapa[res2.code] << " " << res2.res << "; n_steps = " << res2.steps << std::endl;
    res2 = newton(parabola, parabolaDer, parabolaDer2);
    std::cout << "Newton: " << mapa[res2.code] << " " << res2.res << "; n_steps = " << res2.steps << std::endl;
    return 0;
}