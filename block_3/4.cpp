// Задача о скользящей k-статистике (ШАД 1 семестр, 3 контест)

/* По массиву двигаются два указателя — l и r, которые изначально направлены на самый первый элемент массива. Есть две операции: L — сдвинуть указатель l на один элемент вправо и R — сдвинуть указатель r на один элемент вправо. После выполнения каждой из операций нужно вывести k-е в порядке возрастания число среди всех чисел от l до r включительно, либо -1, если всего чисел от l до r меньше, чем k.
Формат ввода
В первой строке входа — три целых числа n, m, k. Во второй строке n целых чисел, задающих массив чисел, по которому будут двигаться два указателя, l и r. В третьей строке входного файла — m символов R или L, без пробелов, в одну строку. Это порядок выполняемых операций. Гарантируется, что указатель l никогда не “обгоняет” указатель r. Гарантируется, что указатель r никогда не выйдет за пределы массива. При этом 1 ≤
n, k ≤ 100000, 0 ≤ m ≤
2n - 2. Все числа в массиве неотрицательные и не превосходят 10^9.
Формат вывода
Выведите ровно m строк, в каждой — ровно по одному целому числу. После выполнения каждой из операций нужно вывести k-е в порядке возрастания число среди всех чисел от l до r включительно, либо -1, если всего чисел от l до r меньше, чем k. */

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

struct MyHeap {
    std::vector<std::pair<int, int>> heap;
    std::vector<int> location;
    bool mx_heap;

    MyHeap(int sz, bool is_heap_max = false) : location(sz), mx_heap(is_heap_max) {}

    int Size(void) const {
        return heap.size();
    }

    int GetMax(void) const {
        if (!mx_heap) {
            std::cout << "Can't get max from min-heap!" << std::endl;
            return -1;
        }
        return heap.empty()? -1 : heap.front().first;
    }

    void Swap(int pos_l, int pos_r) {
        std::swap(location[heap[pos_l].second], location[heap[pos_r].second]);
        std::swap(heap[pos_l], heap[pos_r]);
    } 

    void SiftUp(int idx) {
        if (!idx) {
            return;
        }

        if (mx_heap && heap[(idx - 1) / 2].first < heap[idx].first) {
            Swap((idx - 1) / 2, idx);
            SiftUp((idx - 1) / 2);
        }
        if (!mx_heap && heap[(idx - 1) / 2].first > heap[idx].first) {
            Swap((idx - 1) / 2, idx);
            SiftUp((idx - 1) / 2);
        }
    }

    void SiftDown(size_t idx) {
        if (idx * 2 + 1 >= heap.size()) {
            return;
        }
        int left_son = heap[idx * 2 + 1].first;
        int right_son = (idx * 2 + 2 == heap.size()) ? -1 : heap[idx * 2 + 2].first;
        if (right_son == -1 && !mx_heap) {
            right_son = 2e9;
        }
        if (mx_heap && (left_son > heap[idx].first || right_son > heap[idx].first)) {
            if (left_son > right_son) {
                Swap(idx, idx * 2 + 1);
                SiftDown(idx * 2 + 1);
            } else {
                Swap(idx, idx * 2 + 2);
                SiftDown(idx * 2 + 2);
            }
        } else if (!mx_heap && (left_son < heap[idx].first || right_son < heap[idx].first)) {
            if (left_son < right_son) {
                Swap(idx, idx * 2 + 1);
                SiftDown(idx * 2 + 1);
            } else {
                Swap(idx, idx * 2 + 2);
                SiftDown(idx * 2 + 2);
            }
        }
    }

    void Push(int num, int pos) {
        heap.emplace_back(num, pos);
        location[pos] = heap.size() - 1;
        SiftUp(heap.size() - 1);
    }

    std::pair<int, int> Pop(int pos = -1) {
        int cur_loc = pos == -1? 0 : location[pos];
        Swap(cur_loc, heap.size() - 1);
        auto rv = heap.back();
        heap.pop_back();
        int tmp = heap[cur_loc].second;
        SiftUp(cur_loc);
        SiftDown(location[tmp]);
        return rv;
    }
};

int main(void)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int nn, mm, kk, left = 0, right = 0;
    std::cin >> nn >> mm >> kk;
    std::vector<bool> location(nn);
    std::vector<int> arr(nn);
    MyHeap k_min(nn, true), rest(nn);
    for (int i = 0; i < nn; ++i) {
        std::cin >> arr[i];
    }

    k_min.Push(arr[0], 0);
    location[0] = true;

    for (int idx = 0; idx < mm; ++idx) {
        char turn;
        std::cin >> turn;
        if (turn == 'L') {
            if (location[left]) {
                k_min.Pop(left);
                if (rest.Size()) {
                    auto popped = rest.Pop();
                    k_min.Push(popped.first, popped.second);
                    location[popped.second] = true;
                }
            } else {
                rest.Pop(left);
            }
            ++left;
        } else {
            ++right;
            k_min.Push(arr[right], right);
            location[right] = true;
            if (k_min.Size() > kk) {
                auto popped = k_min.Pop();
                rest.Push(popped.first, popped.second);
                location[popped.second] = false;
            }
        }

        std::cout << (k_min.Size() == kk ? k_min.GetMax() : -1) << std::endl;
    }

    return 0;
}