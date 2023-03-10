#include <cmath>
#include <iostream>
#include <vector>


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


int main() {
    std::vector<std::vector<float>> A = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    std::vector<std::vector<float>> B = { {3, 4, 5, -6, 7}, {2, 1, 0, -2, 5}, {3, 4, 3, 2, 1}, {10, 11, -5, -6, 7}, {2, 8, 3, 5, 9} };
    std::cout << "A: " << determinant(A) << std::endl;  // expected 0
    std::cout << "B: " << determinant(B) << std::endl;  // expected -12030

    return 0;
}