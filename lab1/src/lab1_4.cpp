#include <bitset>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>


float pairwise_sum_simd(float* x, int n) {
    if (n == 1) {
        return x[0];
    }

    int i = 0;
    #pragma omp simd
    for (i = 0; i < n; i += 2) {
        x[i / 2] = i < n - 1 ? x[i] + x[i + 1] : x[i];
    }
    
    return pairwise_sum_simd(x, (n + 1) / 2);
}


float accum_sum(float* x, int n) {
    float res = 0.;
    for (int i = 0; i < n; i++) {
        res += x[i];
    }
    return res;
}


void print(float f) {
    std::cout << std::bitset<32>(*reinterpret_cast<uint32_t*>(&f)) << '\n';
}


int main() {
    srand(time(NULL));
    float* a = new float[1000000];
    int sum = 0;
    int d;

    for (int i = 0; i < 1000000; i++) {
        d = (float)(100 + rand() % 100);
        sum += d;
        a[i] = (float)d / 7.f;
    }
    
    double start1 = clock();
    auto accum = accum_sum(a, 1000000);
    double end1 = clock();
    std::cout << "Accumulative: " << accum << std::endl;
    std::cout << "Accum time: " << (end1 - start1) / CLOCKS_PER_SEC << std::endl;

    double start2 = clock();
    auto pairwise = pairwise_sum_simd(a, 1000000);
    double end2 = clock();
    std::cout << "Pairwise sum: " << pairwise << std::endl;
    std::cout << "Pairwise time: " << (end2 - start2) / CLOCKS_PER_SEC << std::endl;

    std::cout << "\n\n";
    std::cout << "Real sum:  ";
    print((float)sum / 7.f);
    std::cout << "Accum sum: ";
    print(accum);
    std::cout << "Pair sum:  ";
    print(pairwise);

    delete[] a;
    return 0;
}