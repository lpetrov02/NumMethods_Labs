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


float sign(float a) {
    return a >= 0.0 ? 1.0 : -1.0;
}


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


float norm(std::vector<float> x) {
    float res = 0.0;
    for (auto& el : x) {
        res += el * el;
    }
    return res;
}


Result bisection(
    std::function<std::vector<float>(std::vector<float>)> f, 
    std::vector<float> x1, std::vector<float> x2, 
    int maxSteps = 10000, float eps1 = 0.001, float eps2 = 0.001
    ) {
    // f = grad(F) if we want to optimize F
    std::srand(time(NULL));
    int stepsCnt = 0, skippedSteps = 0;

    float dx = 0.0, dy = 0.0;
    float maxDx = 0.0, maxDy = 0.0;
    do {
        maxDx = 0.0, maxDy = 0.0;
        skippedSteps = 0;
        for (int i = 0; i < x1.size(); i++) {
            if (f(x1)[i] * f(x2)[i] > 0) {
                skippedSteps++;
                continue;
            }

            std::vector<float> mid = x1;
            mid[i] = (x1[i] + x2[i]) / 2;
            if (f(x1)[i] * f(mid)[i] < 0) {
                dx = dist(x2, mid);
                dy = abs(norm(f(x2)) - norm(f(mid)));
                x2 = mid;
            }
            else {
                dx = dist(mid, x1);
                dy = abs(norm(f(x1)) - norm(f(mid)));
                x1 = mid;
            }
            if (dx > maxDx)
                maxDx = dx;
            if (dy > maxDy)
                maxDy = dy;
        }
        if (skippedSteps == x1.size()) {
            return Result(x1, 2, stepsCnt);
        }
        stepsCnt++;
    } while (stepsCnt < maxSteps && (maxDx > eps1 || maxDy > eps2));

    if (maxDx > eps1 || maxDy > eps2) {
        return Result(x1, 1, stepsCnt);
    }
    return Result(x1, 0, stepsCnt);
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
        dx = norm(diff);
        dy = abs(f(x) - f(x1));
        x = x1;
        stepsCnt++;

    } while (stepsCnt < maxSteps && (dx > eps1 || dy > eps2));
    if (dx > eps1 || dy > eps2) {
        return Result(x, 1, stepsCnt);
    }
    return Result(x, 0, stepsCnt);
}


float rastrigin(std::vector<float> x) {
    float res = 10.0 * (float)x.size();
    for (auto& el : x) {
        res += el * el - 10 * cos(2 * M_PI * el);
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
    float eps = 0.001, denom = 0.0;
    std::vector<std::vector<float>> res(x.size(), std::vector<float>(x.size(), 0.0));
    for (int i = 0; i < x.size(); i++) {
        denom = 2.0 + 40.0 * M_PI * M_PI * cos(2.0 * M_PI * x[i]);
        res[i][i] = 1.0 / std::max(abs(denom), eps) * sign(denom);
    }
    return res;
}


int main() {
    std::map<int, std::string> mapa = { {0, "SUCCESS"}, {1, "STEPS LIMIT EXCEEDED"}, {2, "ENDLESS LOOP"} };

    std::cout << "Rastrigin's function" << std::endl;

    auto res1 = bisection(rastriginGrad, { -5.5, -5.5 }, { 5.5, 5.5 });
    std::cout << "Bisection: " << mapa[res1.code] << " (";
        for (auto& el : res1.res) {
            std::cout << el << " ";
        } 
    std::cout << "); n_steps = " << res1.steps << std::endl;

    res1 = newton(rastrigin, rastriginGrad, rastriginInvGessian, { -5.5, 5.5 });
    std::cout << "Newton: " << mapa[res1.code] << " (";
    for (auto& el : res1.res) {
        std::cout << el << " ";
    } 
    std::cout << "); n_steps = " << res1.steps << std::endl;

    return 0;
}