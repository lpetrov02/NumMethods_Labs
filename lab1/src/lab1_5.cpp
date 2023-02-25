#include <algorithm>
#include <bitset>
#include <cmath>
#include <iostream>
#include <vector>


using namespace std;

class Statistics {
public:
    Statistics() {
        data = vector<float>();
        _mean = 0.;
        _M = 0.;  // to count variance
        _sum = 0.;
    }

    void update(float x) {
        _sum += x;
        float prevMean = _mean;
        _mean = _mean + (x - _mean) / (data.size() + 1);
        _M += (x - prevMean) * (x - _mean);

        data.push_back(x);
        push_heap(data.begin(), data.end());
    }

    int count() const noexcept {
        return data.size();
    }

    float min() const noexcept {
        return data[0];
    }

    float max() const noexcept {
        return data[data.size() - 1];
    }

    float sum() const noexcept {  // accum sum: fast but not very accurate
        return _sum;
    }

    float pairSum() const {  // much slower (O(1) vs O(n)) but more accurate
        auto storage = data;
        return pairwise_sum_simd(storage, storage.size());
    }

    static float pairwise_sum_simd(vector<float> x, int n) {
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

    float mean() const noexcept {
        return _mean;
    }

    float variance() const noexcept {
        return _M / data.size();
    }

private:
    vector<float> data;
    float _mean;
    float _M;
    float _sum;
};


void print(float f) {
    std::cout << std::bitset<32>(*reinterpret_cast<uint32_t*>(&f)) << '\n';
}


int main() {
    srand(time(NULL));
    Statistics stat = Statistics();
    vector<float> denoms = { 3, 5, 7 };
    for (int i = 0; i < 100; i++) {
        stat.update((float)((i * i * 1234) % 1009) / denoms[rand() % 3]);
    }

    std::cout << "Sum: " << stat.sum() << "; Pair sum: " << stat.pairSum() << std::endl;
    std::cout << "Mean: " << stat.mean() << "; Variance: " << stat.variance() << std::endl;
    std::cout << "Min: " << stat.min() << "; Max: " << stat.max() << std::endl;
    return 0;
}