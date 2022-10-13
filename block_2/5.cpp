#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <cstdlib>
#include <cstdint>

struct TCoin {
    int32_t coord, ttl;

    bool operator<(TCoin rhs) const {
        return std::make_pair(coord, ttl) < std::make_pair(rhs.coord, rhs.ttl);
    }

    TCoin operator+(TCoin rhs) const {
        return {coord + rhs.coord, ttl + rhs.ttl};
    }
};

template <class RandomIt>
inline int32_t Distance(RandomIt from, RandomIt to) {
    return std::abs(to->coord - from->coord);
}

int main() {
    constexpr int32_t kInf = 1'000'000;
    size_t quantity;
    std::cin >> quantity;
    std::vector<TCoin> coins(quantity);
    for (TCoin &item : coins) {
        std::cin >> item.coord >> item.ttl;
    }

    auto dist = [&coins](size_t lhs, size_t rhs) {
        return Distance(std::next(coins.begin(), lhs), std::next(coins.begin(), rhs));
    };
    std::sort(coins.begin(), coins.end());
    std::vector<std::vector<TCoin>> dp(quantity, std::vector<TCoin>(quantity));
    for (size_t shift = 1; shift < quantity; ++shift) {
        for (size_t idx = 0; idx < quantity - shift; ++idx) {
            dp[idx][idx + shift] = {kInf, kInf};
            TCoin distances = dp[idx + 1][idx + shift] + TCoin{dist(idx, idx + 1),
                                                               dist(idx, idx + shift)};
            if (distances.coord <= coins[idx].ttl || distances.ttl <= coins[idx].ttl) {
                dp[idx][idx + shift].coord = std::min(distances.coord, distances.ttl);
            }
            TCoin times = dp[idx][idx + shift - 1] + TCoin{dist(idx, idx + shift),
                                                           dist(idx + shift - 1, idx + shift)};
            if (times.coord <= coins[idx + shift].ttl || times.ttl <= coins[idx + shift].ttl) {
                dp[idx][idx + shift].ttl = std::min(times.coord, times.ttl);
            }
        }
    }
    int32_t ans = std::min(dp.front().back().coord, dp.front().back().ttl);

    if (ans == kInf) {
        std::cout << "No solution";
    } else {
        std::cout << ans;
    }
    std::cout << std::endl;

    return 0;
}