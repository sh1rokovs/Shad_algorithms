#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int64_t module = 123456789;
    int64_t size_vec;
    std::cin >> size_vec;
    std::vector<int64_t> vec_keys(size_vec);
    for (int64_t i = 0; i < size_vec; ++i) {
        std::cin >> vec_keys[i];
    }
    std::sort(vec_keys.begin(), vec_keys.end());
    std::vector<std::vector<int64_t>> unique_vec(size_vec, std::vector<int64_t>(size_vec, 1));
    int64_t len = 1;
    for (; len < size_vec; ++len) {
        for (int64_t left = 0; left + len < size_vec; ++left) {
            int64_t right = left + len;
            unique_vec[left][right] = unique_vec[left + 1][right];
            if (vec_keys[right] != vec_keys[right - 1]) {
                unique_vec[left][right] =
                    (unique_vec[left][right] + unique_vec[left][right - 1]) % module;
            }
            int64_t inner = left + 1;
            for (; inner < right; ++inner) {
                if (inner > left && vec_keys[inner] == vec_keys[inner - 1]) {
                    continue;
                }
                unique_vec[left][right] +=
                    (unique_vec[left][inner - 1] * unique_vec[inner + 1][right]) % module;
                unique_vec[left][right] %= module;
            }
        }
    }
    std::cout << unique_vec[0][size_vec - 1] << std::endl;
    return 0;
}