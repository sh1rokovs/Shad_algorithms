#include <iostream>
#include <vector>
#include <iomanip>
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t elements, height;
    size_t max_el = 101;
    std::cin >> elements >> height;
    std::vector<std::vector<long double>> prob(max_el, std::vector<long double>(max_el, 0.0));
    if (height >= elements) {
        std::cout << std::setprecision(5) << 0 << '\n';
        return 0;
    }
    for (size_t i = 0; i < elements + 1; ++i) {
        prob[i][0] = (i == 0) ? 1.0 : 0.0;
    }
    prob[1][0] = 1.0;
    prob[1][1] = 0.0;
    if (height >= elements) {
        std::cout << std::setprecision(5) << 0 << '\n';
        return 0;
    }
    long double temp_prob;
    for (size_t num = 2; num <= elements; ++num) {
        for (size_t h = 1; h <= height; ++h) {
            temp_prob = 0.0;
            for (size_t k = 0; k <= num - 1; ++k) {
                for (size_t m = 0; m < h - 1; ++m) {
                    temp_prob += 2.0 * prob[k][h - 1] * prob[num - k - 1][m];
                }
            }
            for (size_t k = 0; k <= num - 1; ++k) {
                temp_prob += prob[k][h - 1] * prob[num - k - 1][h - 1];
            }
            prob[num][h] = temp_prob / num;
        }
    }
    long double answer = prob[elements][height];
    std::cout << std::setprecision(10) << answer << '\n';
}