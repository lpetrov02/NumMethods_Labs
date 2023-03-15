#include <functional>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <map>
#include <string>


struct Result {
    Result(std::vector<float> r, int c, int s) : res(r), code(c), steps(s) {}
    std::vector<float> res;
    int code;
    int steps;
};


float abs(float a) {
    return a >= 0 ? a : -a;
}


// Result bisection(std::function<float(float)> f, int maxSteps = 10000, float eps1 = 0.001, float eps2 = 0.001) {
//     // f = F' if we want to optimize F
//     std::srand(time(NULL));
//     float x1 = -10.0, x2 = 10.0;
//     int maxSteps0 = 100;
//     int stepsCnt = 0;
//     while (f(x1) >= 0 && stepsCnt < maxSteps0) {
//         x1 -= (float)(std::rand() % 1000) / 10;
//         stepsCnt++;
//     }
//     while (f(x2) <= 0 && stepsCnt < maxSteps0) {
//         x2 += (float)(std::rand() % 1000) / 10;
//         stepsCnt++;
//     }
//     if (f(x1) * f(x2) >= 0) {
//         return Result(0, 2, -1);
//     }

//     float dx = 0.0, dy = 0.0;
//     stepsCnt = 0;
//     do {
//         float mid = (x1 + x2) / 2;
//         if (f(x1) * f(mid) < 0) {
//             dx = x2 - mid;
//             dy = abs(f(x2) - f(mid));
//             x2 = mid;
//         }
//         else {
//             dx = mid - x1;
//             dy = abs(f(x1) - f(mid));
//             x1 = mid;
//         }
//         stepsCnt++;

//     } while (stepsCnt < maxSteps && (dx > eps1 || dy > eps2));

//     if (dx > eps1 || dy > eps2) {
//         return Result(x1, 1, stepsCnt);
//     }
//     return Result(x1, 0, stepsCnt);
// }


std::vector<float> matVecMul(
    std::vector<std::vector<float>> H,
    std::vector<float> g
    ) {
        std::vector<float> res;
        for (auto& row : H) {
            float s = 0.0;
            for (int i = 0; i < row.size(); i++) {
                s += row[i] * g[i];
            }
            res.push_back(s);
        }
        return res;
    }


float dist(std::vector<float> a, std::vector<float> b) {
    float res = 0.0;
    for (int i = 0; i < a.size(); i++) {
        res += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(res);
}


Result newton(
    std::function<float(std::vector<float>)> f, 
    std::function<std::vector<float>(std::vector<float>)> grad, 
    std::function<std::vector<std::vector<float>>(std::vector<float>)> gessianInv, 
    std::vector<float> x,
    int maxSteps = 10000, float eps1 = 0.001, float eps2 = 0.001
    ) {
    int stepsCnt = 0;

    float dx = 0.0, dy = 0.0;
    std::vector<float> x1(x.size());
    stepsCnt = 0;
    do {
        auto diff = matVecMul(gessianInv(x), grad(x));
        for (int i = 0; i < x.size(); i++) {
            x1[i] = x[i] - diff[i];
        }
        dx = dist(x, x1);
        dy = abs(f(x) - f(x1));
        x = x1;
        stepsCnt++;

    } while (stepsCnt < maxSteps && (dx > eps1 || dy > eps2));
    std::cout << dx << " " << dy << std::endl;
    if (dx > eps1 || dy > eps2) {
        return Result(x, 1, stepsCnt);
    }
    return Result(x, 0, stepsCnt);
}


float rastrigin(std::vector<float> x) {
    float res = 10.0 * (float)x.size();
    for (auto& el : x) {
        res += el * el;
        res -= 10 * cos(2 * M_PI * el);
    }
    return res;
}

std::vector<float> rastriginGrad(std::vector<float> x) {
    std::vector<float> res;
    for (auto& el : x) {
        res.push_back(2 * el + 20 * M_PI * sin(2 * M_PI * el));
    }
    return res;
}

std::vector<std::vector<float>> rastriginInvGessian(std::vector<float> x) {
    std::vector<std::vector<float>> res(x.size(), std::vector<float>(x.size(), 0.0));
    for (int i = 0; i < x.size(); i++) {
        res[i][i] = 1.0 / (2.0 + 40.0 * M_PI * M_PI * cos(2.0 * M_PI * x[i]));
    }
    return res;
}


int main() {
    std::map<int, std::string> mapa = { {0, "SUCCESS"}, {1, "ITER LIMIT EXCEEDED"}, {2, "INIT FAILURE"} };

    std::cout << "Rastrigin's function" << std::endl;
    // auto res1 = bisection(fooDer);
    // std::cout << "Bisection: " << mapa[res1.code] << " " << res1.res << "; n_steps = " << res1.steps << std::endl;
    auto res1 = newton(rastrigin, rastriginGrad, rastriginInvGessian, { 4.0, 4.0 }, 10000, 0.0000001, 0.0000001);
    std::cout << "Newton: " << mapa[res1.code] << " (";
    for (auto& el : res1.res) {
        std::cout << el << " ";
    } 
    std::cout << "); n_steps = " << res1.steps << std::endl;
    return 0;
}