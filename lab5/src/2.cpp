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


float findParabolaMax(std::function<float(float)> f, std::vector<float> pts, int idx) {    
    float a = 0.0, b = 0.0, s = pts[idx - 1] + pts[idx] + pts[idx + 1];

    for (int i = idx - 1; i < idx + 2; i++) {
        float prod = 1.f;
        for (int j = idx - 1; j < idx + 2; j++) {
            if (j == i) {
                continue;
            }
            prod /= (pts[i] - pts[j]);
        }
        a += prod * f(pts[i]);
        b -= (s - pts[i]) * prod * f(pts[i]);
    }

    return -b / 2.f / a;
}


int main() {
    float xMin = -10.f - 13.f * M_PI / 30.f, xMax = 10.f;
    float step = M_PI / 5.f;
    std::vector<float> pts;
    for (float x = xMin; x < xMax; x += step) {
        pts.push_back(x);
    }

    for (int i = 1; i < pts.size() - 1; i++) {
        if (sin(pts[i]) > sin(pts[i - 1]) && sin(pts[i]) > sin(pts[i + 1])) {
            std::cout << findParabolaMax(sin, pts, i) << std::endl;
        }
    }

    return 0;
}