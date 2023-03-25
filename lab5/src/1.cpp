#include <functional>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>


struct Result {
    Result(float r, int s) : res(r), status(s) {}
    float res;
    int status;
};


struct PtWithDist {
    PtWithDist(float x, float d) : dist(d), pt(x) {}
    float pt;
    float dist;

    bool operator<(PtWithDist const& other) {
        return dist < other.dist;
    }
};


float abs(float a) {
    return a >= 0 ? a : -a;
}


float sign(float a) {
    return a >= 0 ? 1.0 : -1.0;
}


float lagrangeInterpolation(std::function<float(float)> f, std::vector<float> pts, int n, float x) {
    std::vector<PtWithDist> neighbours;
    for (auto& p : pts) {
        neighbours.push_back(PtWithDist(p, abs(p - x)));
        std::push_heap(neighbours.begin(), neighbours.end());
        if (neighbours.size() > n) {
            std::pop_heap(neighbours.begin(), neighbours.end());
            neighbours.pop_back();
        }
    }

    float res = 0.0;
    for (int i = 0; i < n; i++) {
        float prod = 1.f;
        for (int j = 0; j < n; j++) {
            if (j == i) {
                continue;
            }
            prod *= (x - neighbours[j].pt) / (neighbours[i].pt - neighbours[j].pt);
        }
        res += prod * f(neighbours[i].pt);
    }
    return res;
}


float countScore(std::function<float(float)> f, std::vector<float> pts, int n) {
    float score = 0.0, p;
    int cnt = 0;

    for (int i = 0; i < pts.size() - 1; i++) {
        p = (pts[i] + pts[i + 1]) / 2.f;
        score += abs(f(p) - lagrangeInterpolation(f, pts, n, p));
        cnt++;
    }

    return score / (float)cnt;
}


int main() {
    float xMin = -10.f - 13.f * M_PI / 30.f, xMax = 10.f;
    float step = M_PI / 5.f;
    std::vector<float> pts;
    for (float x = xMin; x < xMax; x += step) {
        pts.push_back(x);
    }
    std::cout << "Step for 2nd degree interpolation = " << step << std::endl;
    std::cout << "Poly(deg = 2) score: " << countScore(sin, pts, 2) << std::endl << std::endl;

    for (int i = 0; i < 8; i++) {
        std::cout << "Step for linear interpolation = " << step << std::endl;
        pts.clear();
        for (float x = xMin; x < xMax; x += step) {
            pts.push_back(x);
        }

        std::cout << "Linear score: " << countScore(sin, pts, 1) << std::endl << std::endl;

        step /= 2.f;
    }

    return 0;
}