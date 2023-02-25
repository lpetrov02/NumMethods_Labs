#include <cmath>
#include <iostream>


float length(const float* x, int n) {
    float maxElement = 1.;
    float sum = 0;
    for (int i = 0; i < n; i++) {
        if (x[i] <= maxElement) {
            sum += (x[i] / maxElement) * (x[i] / maxElement);
        }
        else {
            sum = sum / (x[i] * x[i]) * (maxElement * maxElement);
            sum += 1.;
            maxElement = x[i];
        }
    }
    return maxElement * sqrt(sum);
}


int main() {
    int N = 100;

    float* a = new float[N];
    for (int i = 0; i < N; i++) {
        a[i] = i;
    }

    std::cout << length(a, N) << std::endl;

    delete[] a;
    return 0;
}