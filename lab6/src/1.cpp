#include <functional>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>



struct Point {
    float x;
    float y;
    float z;

    Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Point() : Point(0.0, 0.0, 0.0) {}
};


struct PtWithDist {
    Point pt;
    float dist;

    PtWithDist(Point p, float d) : dist(d), pt(p) {}

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


std::vector<Point> parseFile(std::string pathToFile) {
    std::vector<Point> res;
    std::ifstream input(pathToFile);
    std::string s;
    float x, y, z;

    while (getline(input, s)) {
        int i = 0;

        std::stringstream ss(s);
        ss >> x >> y >> z;
        res.push_back(Point(x, y, z));
    }

    return res;
}


float baricentricInterpolation(std::vector<Point> pts, float x, float y) {
    int N = 3;
    std::vector<PtWithDist> pt;
    for (auto& p : pts) {
        pt.push_back(PtWithDist(p, (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y)));
        std::push_heap(pt.begin(), pt.end());
        if (pt.size() > N) {
            std::pop_heap(pt.begin(), pt.end());
            pt.pop_back();
        }
    }

    float d = (pt[1].pt.y - pt[2].pt.y) * (pt[0].pt.x - pt[2].pt.x) + (pt[0].pt.y - pt[2].pt.y) * (pt[2].pt.x - pt[1].pt.x);
    float t1, t2, t3;
    t1 = ((pt[1].pt.y - pt[2].pt.y) * (x - pt[2].pt.x) + (y - pt[2].pt.y) * (pt[2].pt.x - pt[1].pt.x)) / d;
    t2 = ((pt[2].pt.y - pt[0].pt.y) * (x - pt[2].pt.x) + (y - pt[2].pt.y) * (pt[0].pt.x - pt[2].pt.x)) / d;
    t3 = 1.f - t1 - t2;
    return t1 * pt[0].pt.z + t2 * pt[1].pt.z + t3 * pt[2].pt.z;
}


int main() {
    std::vector<Point> pts = parseFile("surface.xyz");
    float x = 0.123, y = -2.456;
    std::cout << "Interpolated point: (" << x << ", " << y << ", " << baricentricInterpolation(pts, x, y) << ")" << std::endl;

    return 0;
}