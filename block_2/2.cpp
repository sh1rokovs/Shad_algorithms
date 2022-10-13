#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>
#include <utility>
#include <cstdint>

namespace heap {
template<class RandomIt>
inline RandomIt LeftOffspring(RandomIt first, RandomIt item);

template<class RandomIt>
inline RandomIt RightOffspring(RandomIt first, RandomIt item);

template<class RandomIt, class Compare>
void SiftDown(RandomIt first, RandomIt item, RandomIt last, Compare comp);

template<class RandomIt, class Compare>
void MakeHeap(RandomIt first, RandomIt last, Compare comp);

template<class RandomIt, class Compare>
void Sort(RandomIt first, RandomIt last, Compare comp);
} // namespace heap

struct TPlayer {
    size_t idx;
    uint32_t eff;

    static bool CompareByIndex(TPlayer lhs, TPlayer rhs);
    static bool CompareByEffectiveness(TPlayer lhs, TPlayer rhs);
};

template<class RandomIt>
class TTeam {
private:
    RandomIt first_, last_;

public:
    TTeam(RandomIt first, RandomIt last);

    [[nodiscard]] bool Empty() const;
    [[nodiscard]] size_t Size() const;

    [[nodiscard]] bool Correct() const;

    RandomIt Begin() const;
    RandomIt End() const;
};

template<class RandomIt>
uint64_t TeamEffectiveness(TTeam<RandomIt> team);

std::vector<TPlayer> BuildMostEffectiveSolidaryTeam(std::vector<TPlayer> players);

std::vector<TPlayer> ReadPlayers(std::istream &is);

void PrintTeam(std::ostream &os, std::vector<TPlayer> res);

int main() {
    std::vector<TPlayer> players = ReadPlayers(std::cin);
    std::vector<TPlayer> best_team = BuildMostEffectiveSolidaryTeam(players);
    PrintTeam(std::cout, best_team);
    return 0;
}

template<class RandomIt>
inline RandomIt heap::LeftOffspring(RandomIt first, RandomIt item) {
    return std::next(first, 2 * std::distance(first, item) + 1);
}

template<class RandomIt>
inline RandomIt heap::RightOffspring(RandomIt first, RandomIt item) {
    return std::next(first, 2 * std::distance(first, item) + 2);
}

template<class RandomIt, class Compare>
void heap::SiftDown(RandomIt first, RandomIt item, RandomIt last, Compare comp) {
    auto offsprings = [first](RandomIt vertex) {
        return std::make_pair(
            heap::LeftOffspring(first, vertex),
            heap::RightOffspring(first, vertex)
        );
    };

    auto[lhs, rhs] = offsprings(item);
    while (rhs < last && (comp(*item, *lhs) || comp(*item, *rhs))) {
        if (comp(*lhs, *rhs)) {
            std::swap(*item, *rhs);
            item = rhs;
        } else {
            std::swap(*item, *lhs);
            item = lhs;
        }
        std::tie(lhs, rhs) = offsprings(item);
    }
    if (lhs < last && comp(*item, *lhs)) {
        std::swap(*item, *lhs);
    }
}

template<class RandomIt, class Compare>
void heap::MakeHeap(RandomIt first, RandomIt last, Compare comp) {
    using heap::SiftDown;

    const size_t size = last - first;
    for (size_t border = (last - first + 1) / 2; border < size; --border) {
        SiftDown(first, std::next(first, border), last, comp);
    }
}

template<class RandomIt, class Compare>
void heap::Sort(RandomIt first, RandomIt last, Compare comp) {
    using heap::MakeHeap;
    using heap::SiftDown;

    MakeHeap(first, last, comp);
    for (size_t i = last - first; i--; SiftDown(first, first, std::next(first, i), comp)) {
        std::swap(*first, first[i]);
    }
}

bool TPlayer::CompareByIndex(TPlayer lhs, TPlayer rhs) {
    return lhs.idx < rhs.idx;
}

bool TPlayer::CompareByEffectiveness(TPlayer lhs, TPlayer rhs) {
    return lhs.eff < rhs.eff;
}

template<class RandomIt>
TTeam<RandomIt>::TTeam(RandomIt first, RandomIt last) : first_(first), last_(last) {
}

template<class RandomIt>
bool TTeam<RandomIt>::Empty() const {
    return !Size();
}

template<class RandomIt>
size_t TTeam<RandomIt>::Size() const {
    return End() - Begin();
}

template<class RandomIt>
bool TTeam<RandomIt>::Correct() const {
    if (Size() < 3) {
        return true;
    }
    return std::prev(End())->eff <= Begin()->eff + std::next(Begin())->eff;
}

template<class RandomIt>
RandomIt TTeam<RandomIt>::Begin() const {
    return first_;
}

template<class RandomIt>
RandomIt TTeam<RandomIt>::End() const {
    return last_;
}

template<class RandomIt>
uint64_t TeamEffectiveness(TTeam<RandomIt> team) {
    return std::accumulate(team.Begin(), team.End(), 0ull, [](uint64_t lhs, TPlayer rhs) {
        return lhs + rhs.eff;
    });
}

std::vector<TPlayer> BuildMostEffectiveSolidaryTeam(std::vector<TPlayer> players) {
    if (players.size() < 3) {
        return players;
    }

    heap::Sort(players.begin(), players.end(), TPlayer::CompareByEffectiveness);
    uint64_t best_eff = 0, current_eff = 0;
    TTeam best_team(players.begin(), players.begin());
    TTeam current_team = best_team;
    do {
        if (current_team.End() != players.end()) {
            current_eff += current_team.End()->eff;
            current_team = TTeam(current_team.Begin(), std::next(current_team.End()));
        }
        while (!current_team.Empty() && !current_team.Correct()) {
            current_eff -= current_team.Begin()->eff;
            current_team = TTeam(std::next(current_team.Begin()), current_team.End());
        }
        if (best_eff < current_eff) {
            best_eff = current_eff;
            best_team = current_team;
        }
    } while (current_team.Begin() < players.end() && current_team.End() < players.end());
    return {best_team.Begin(), best_team.End()};
}

std::vector<TPlayer> ReadPlayers(std::istream &is) {
    size_t quantity;
    is >> quantity;
    std::vector<TPlayer> res;
    res.reserve(quantity);
    for (size_t idx = 0; idx < quantity; ++idx) {
        uint32_t effectiveness;
        is >> effectiveness;
        res.push_back({idx, effectiveness});
    }
    return res;
}

void PrintTeam(std::ostream &os, std::vector<TPlayer> res) {
    heap::Sort(res.begin(), res.end(), TPlayer::CompareByIndex);
    os << TeamEffectiveness(TTeam(res.begin(), res.end())) << '\n';
    for (TPlayer player: res) {
        os << player.idx + 1 << ' ';
    }
    os << std::endl;
}