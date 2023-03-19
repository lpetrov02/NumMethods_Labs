#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


struct Point {
    float x;
    float y;
    float z;

    Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Point() : Point(0.0, 0.0, 0.0) {}
};


float abs(float x) {
    return x >= 0 ? x : -x;
}


std::vector<Point> parseFile(std::string pathToFile) {
    std::vector<Point> res;
    std::ifstream input(pathToFile);
    std::string s;
    float x, y, z;

    while (getline(input, s)) {
        int i = 0;
        while (s[i] == ' ' || s[i] == '\t') {
            i++;
        }
        if (s[i] != 'v') {
            continue;
        }
        while (s[i++] != ' ') {}

        std::stringstream ss(s.substr(i));
        ss >> x >> y >> z;
        res.push_back(Point(x, y, z));
    }

    return res;
}


float det(std::vector<std::vector<float>> A, int level, std::vector<int> idxs, int coef) {
    int n = idxs.size();
    if (level == n) {
        float res = (float)coef;
        for (int i = 0; i < n; i++) {
            res *= A[i][idxs[i]];
        }

        return res;
    }

    float accum = 0;
    for (int i = n - 1; i >= 0; i--) {
        if (0 <= idxs[i] && idxs[i] < level) {
            coef *= -1;
        }
        if (idxs[i] == -1) {
            idxs[i] = level;
            accum += det(A, level + 1, idxs, coef);
            idxs[i] = -1;
        }
    }
    return accum;
}


float determinant(std::vector<std::vector<float>> A) {
    int n = A.size();
    std::vector<int> idxs(n, -1);
    return det(A, 0, idxs, 1);
}


std::vector<std::vector<float>> makeMat(Point a, Point b, Point c, Point d) {
    std::vector<std::vector<float>> res(4, std::vector<float>(4, 0.0));
    std::vector<Point> pts = { a, b, c, d };
    for (int i = 0; i < 4; i++) {
        res[0][i] = pts[i].x;
        res[1][i] = pts[i].y;
        res[2][i] = pts[i].z;
        res[3][i] = 1.0;
    }

    return res;
}


float evalVolume(std::vector<Point> pts, float minz, float dx, float dy, float dz, float h) {
    float eps = 0.001;
    h += minz + eps;

    float vol = 0.0;
    Point d(dx, dy, dz);
    for (int i = 0; i < pts.size(); i += 3) {
        if (pts[i].z > h && pts[i + 1].z > h && pts[i + 2].z > h) {
            continue;
        }
        auto mat = makeMat(pts[i], pts[i + 1], pts[i + 2], d);
        vol += determinant(mat) / 6.0;
    }

    return abs(vol);
}


template<class T>
void swap(T* p1, T* p2) {
    T tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}


float evalVolumeIq1000(std::vector<Point> pts, float minz, float dx, float dy, float dz, float h) {
    h += minz;

    float vol = 0.0;
    Point d(dx, dy, dz);
    for (int i = 0; i < pts.size(); i += 3) {
        std::vector<int> idxs = { 0, 1, 2 };
        if (pts[i].z < pts[i + 1].z) { swap<Point>(&pts[i], &pts[i + 1]); swap<int>(&idxs[0], &idxs[1]); }
        if (pts[i + 1].z < pts[i + 2].z) { swap<Point>(&pts[i + 1], &pts[i + 2]); swap<int>(&idxs[1], &idxs[2]); }
        if (pts[i].z < pts[i + 1].z) { swap<Point>(&pts[i], &pts[i + 1]); swap<int>(&idxs[0], &idxs[1]); }
        // Here we have a kind of bubble sort but without any function 
        // Now p1.z >= p2.z >= p3.z

        if (pts[i].z >= h && pts[i + 1].z >= h && pts[i + 2].z >= h) {
            continue;
        }
        else if (pts[i].z < h && pts[i + 1].z < h && pts[i + 2].z < h) {
            std::vector<Point> pts2(3);
            for (int k = 0; k < 3; k++) {
                pts2[idxs[k]] = pts[i + k];
            }
            auto mat = makeMat(pts2[0], pts2[1], pts2[2], d);
            vol += determinant(mat) / 6.0;
        }
        else if (pts[i].z >= h && pts[i + 1].z < h && pts[i + 2].z < h) {
            float c1 = (h - pts[i + 1].z) / (pts[i].z - pts[i + 1].z), c2 = (h - pts[i + 2].z) / (pts[i].z - pts[i + 2].z);
            Point A1(pts[i].x * c1 + pts[i + 1].x * (1.0 - c1), pts[i].y * c1 + pts[i + 1].y * (1.0 - c1), h);
            Point A2(pts[i].x * c2 + pts[i + 2].x * (1.0 - c2), pts[i].y * c2 + pts[i + 2].y * (1.0 - c2), h);
            std::vector<Point> pts2(3);
            for (int k = 0; k < 3; k++) {
                pts2[idxs[k]] = pts[i + k];
            }
            auto matPlus = makeMat(pts2[0], pts2[1], pts2[2], d);
            vol += determinant(matPlus) / 6.0;

            for (int k = 0; k < 3; k++) {
                if (idxs[k] == 1)
                    pts2[k] = A1;
                if (idxs[k] == 2)
                    pts2[k] = A2;
                else
                    pts2[k] = pts[i];
            }
            auto matMinus = makeMat(pts2[0], pts2[1], pts2[2], d);
            vol -= determinant(matMinus) / 6.0;
        }
        else {
            float c1 = (pts[i].z - h) / (pts[i].z - pts[i + 2].z), c2 = (pts[i + 1].z - h) / (pts[i + 1].z - pts[i + 2].z);
            Point A1(pts[i + 2].x * c1 + pts[i].x * (1.0 - c1), pts[i + 2].y * c1 + pts[i].y * (1.0 - c1), h);
            Point A2(pts[i + 2].x * c2 + pts[i + 1].x * (1.0 - c2), pts[i + 2].y * c2 + pts[i + 1].y * (1.0 - c2), h);
            std::vector<Point> pts2(3);
            for (int k = 0; k < 3; k++) {
                pts2[idxs[k]] = pts[i + k];
                if (idxs[k] == 0)
                    pts2[k] = A1;
                else if (idxs[k] == 1)
                    pts2[k] = A2;
                else
                    pts2[k] = pts[i + 2];
            }
            auto mat = makeMat(pts2[0], pts2[1], pts2[2], d);
            vol += determinant(mat) / 3.0;
        }
    }

    return abs(vol);
}


float evalLevel(std::vector<Point> pts, float minz, float maxz, float volume, float dx, float dy, float dz) {
    float eps = 0.001;
    float lo = minz, hi = maxz, mid, volMid;
    while (hi - lo > 0.001) {
        mid = (lo + hi) / 2.0;
        volMid = evalVolume(pts, minz, dx, dy, dz, mid - minz);
        if (volMid > volume) {
            hi = mid;
        }
        else {
            lo = mid;
        }
    }
    return lo;
}


float evalLevelIq1000(std::vector<Point> pts, float minz, float maxz, float volume, float dx, float dy, float dz) {
    float eps = 0.001;
    float lo = minz, hi = maxz, mid, volMid;
    while (hi - lo > 0.001) {
        mid = (lo + hi) / 2.0;
        volMid = evalVolumeIq1000(pts, minz, dx, dy, dz, mid - minz);
        if (volMid > volume) {
            hi = mid;
        }
        else {
            lo = mid;
        }
    }
    return lo;
}


int main() {
    auto pts = parseFile("tank.stl");
    float minz = 1000.0, maxz = -1000.0, minx = 1000.0, maxx = -1000.0, miny = 1000.0, maxy = -1000.0;
    for (auto& p : pts) {
        if (p.z < minz)
            minz = p.z;
        if (p.z > maxz)
            maxz = p.z;
        if (p.x < minx)
            minx = p.x;
        if (p.x > maxx)
            maxx = p.x;
        if (p.y < miny)
            miny = p.y;
        if (p.y > maxy)
            maxy = p.y;
    }
    float dx = (minx + maxx) / 2.0, dy = (miny + maxy) / 2.0, dz = (minz + maxz) / 2.0;

    std::cout << "Minimum z: " << minz << "; Maximum z: " << maxz << std::endl;
    std::cout << "Iq 0: Half volume: " << evalVolume(pts, minz, dx, dy, dz, (maxz - minz) / 2) << std::endl;
    std::cout << "Iq 0: Full volume: " << evalVolume(pts, minz, dx, dy, dz, maxz - minz) << std::endl;
    std::cout << "Iq 1000: Half volume: " << evalVolumeIq1000(pts, minz, dx, dy, dz, (maxz - minz) / 2) << std::endl;
    std::cout << "Iq 1000: Full volume: " << evalVolumeIq1000(pts, minz, dx, dy, dz, maxz - minz) << std::endl;

    std::cout << "Iq 0: Level for vol 200000: " << evalLevel(pts, minz, maxz, 200000.0, dx, dy, dz) << std::endl;
    std::cout << "Iq 1000: Level for vol 200000: " << evalLevelIq1000(pts, minz, maxz, 200000.0, dx, dy, dz) << std::endl;
    return 0;
}