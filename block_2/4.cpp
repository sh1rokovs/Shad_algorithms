#include <iostream>
#include <algorithm>
#include <random>
#include <utility>
#include <vector>
#include <cstdint>

class Random32Generator {
private:
    uint32_t current_;
    std::pair<uint32_t, uint32_t> parameters_;

    uint32_t nextRand24();

public:
    Random32Generator(std::pair<uint32_t, uint32_t>);

    uint32_t nextRand32();
};

namespace own {
    template <class RandomIt>
    void nth_element(RandomIt first, RandomIt nth, RandomIt last) {
        using T = typename RandomIt::value_type;
        static constexpr size_t kSeed = 938;
        static std::mt19937 gen(kSeed);
        auto index = [first](RandomIt it) { return std::distance(first, it); };

        RandomIt left = first, right = std::prev(last);
        for (RandomIt lhs, rhs; left != right; nth < lhs ? (right = rhs) : (left = lhs)) {
            std::uniform_int_distribution distr(index(left), index(right));
            T pivot = first[distr(gen)];
            lhs = left, rhs = right;
            while (lhs <= rhs) {
                while (*lhs < pivot) {
                    ++lhs;
                }
                while (pivot < *rhs) {
                    --rhs;
                }
                if (lhs <= rhs) {
                    std::swap(*lhs++, *rhs--);
                }
            }
            if (nth < lhs && rhs < nth) {
                return;
            }
        }
    }
} // namespace own

int main() {
    size_t quantity;
    std::cin >> quantity;
    std::vector<int64_t> houses(quantity);

    std::pair<uint32_t, uint32_t> parameters;
    std::cin >> parameters.first >> parameters.second;
    Random32Generator gen(parameters);

    for (int64_t &item : houses) {
        item = gen.nextRand32();
    }
    own::nth_element(houses.begin(), std::next(houses.begin(), houses.size() >> 1), houses.end());
    int64_t distance = 0;
    for (int64_t coordinate : houses) {
        distance += std::abs(houses[houses.size() >> 1] - coordinate);
    }
    std::cout << distance << std::endl;

    return 0;
}

uint32_t Random32Generator::nextRand24() {
    current_ = current_ * parameters_.first + parameters_.second;
    return current_ >> 8;
}

Random32Generator::Random32Generator(std::pair<uint32_t, uint32_t> parameters)
    : current_(0), parameters_(std::move(parameters)) {
}

uint32_t Random32Generator::nextRand32() {
    uint32_t first = nextRand24(), second = nextRand24();
    return (first << 8) ^ second;
}