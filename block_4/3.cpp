#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
void ToPostorder(std::vector<int> &preorder, int min, int max, size_t &idx) {
    if (idx == preorder.size()) {
        return;
    } else if (preorder[idx] < min) {
        return;
    } else if (preorder[idx] >= max) {
        return;
    }
    int val = preorder[idx];
    ++idx;
    ToPostorder(preorder, min, val, idx);
    ToPostorder(preorder, val, max, idx);
    std::cout << val << " ";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t size;
    std::cin >> size;
    if (size == 0) {
        return 0;
    }
    std::vector<int> arr(size);
    for (auto &val : arr) {
        std::cin >> val;
    }
    size_t idx = 0;
    ToPostorder(arr, std::numeric_limits<int>::min(),
                std::numeric_limits<int>::max(), idx);
    std::cout << "\n";
    std::sort(arr.begin(), arr.end());
    for (auto val : arr) {
        std::cout << val << " ";
    }
    std::cout << "\n";
    return 0;
}