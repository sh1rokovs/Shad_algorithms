#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <utility>

template <typename T, class Compare>
class PriorityQueue {
private:
    std::vector<T> containter_{};
    Compare comparator_{};

public:
    bool Empty() const;
    const T& Top() const;

    void Push(T);
    void Pop();
};

int main() {
    size_t rows, columns;
    std::cin >> rows >> columns;
    std::vector<std::vector<int>> sequences(rows, std::vector<int>(columns));
    for (std::vector<int> &row : sequences) {
        for (int &item : row) {
            std::cin >> item;
        }
    }

    std::vector<size_t> indices(rows, 0);
    PriorityQueue<std::pair<int, size_t>, std::greater<std::pair<int, size_t>>> heap;
    for (size_t i = 0; i < rows; ++i) {
        heap.Push(std::make_pair(sequences[i][indices[i]], i));
    }
    while (!heap.Empty()) {
        std::cout << heap.Top().first << ' ';
        size_t idx = heap.Top().second;
        heap.Pop();
        ++indices[idx];
        if (indices[idx] < columns) {
            heap.Push(std::make_pair(sequences[idx][indices[idx]], idx));
        }
    }
    std::cout << std::endl;

    return 0;
}

template <typename T, class Compare>
bool PriorityQueue<T, Compare>::Empty() const {
    return containter_.empty();
}

template <typename T, class Compare>
const T& PriorityQueue<T, Compare>::Top() const {
    return containter_.front();
}

template <typename T, class Compare>
void PriorityQueue<T, Compare>::Push(T obj) {
    containter_.emplace_back(std::move(obj));
    std::push_heap(containter_.begin(), containter_.end(), comparator_);
}

template <typename T, class Compare>
void PriorityQueue<T, Compare>::Pop() {
    std::pop_heap(containter_.begin(), containter_.end(), comparator_);
    containter_.pop_back();
}