#include <iostream>
#include <algorithm>
#include <vector>

enum class EExpected {
    Less,
    Greater,
    Nevermind
};

struct DynamicItem {
    size_t length;
    ssize_t prev_idx;
    EExpected state;
};

bool operator<(const DynamicItem &lhs, const DynamicItem &rhs) {
    return lhs.length < rhs.length;
}

bool operator<(const std::vector<size_t> &lhs, const std::vector<size_t> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

std::vector<size_t> get_indices_sequence(const std::vector<DynamicItem> &dyn_res, size_t idx) {
    std::vector<size_t> res;
    res.reserve(dyn_res[idx].length);
    while (dyn_res[idx].prev_idx != -1l) {
        res.push_back(idx);
        idx = dyn_res[idx].prev_idx;
    }
    res.push_back(idx);
    std::reverse(res.begin(), res.end());
    return res;
}

std::vector<DynamicItem> dynamic_solution(const std::vector<int> &seq) {
    std::vector<DynamicItem> ans(seq.size(), { 1ul, -1l, EExpected::Nevermind });
    for (size_t i_ = 1; i_ < seq.size(); ++i_) {
        for (size_t j_ = 0; j_ < i_; ++j_) {
            switch (ans[j_].state) {
            case EExpected::Nevermind:
            case EExpected::Less:
                if (seq[i_] < seq[j_] && ans[i_].length < ans[j_].length + 1) {
                    ans[i_] = { ans[j_].length + 1, static_cast<ssize_t>(j_), EExpected::Greater };
                } else if (seq[i_] < seq[j_] && ans[i_].length == ans[j_].length + 1) {
                    std::vector<size_t> cur_seq = get_indices_sequence(ans, i_);
                    std::vector<size_t> potentially_seq = get_indices_sequence(ans, j_);
                    potentially_seq.push_back(i_);
                    if (potentially_seq < cur_seq) {
                        ans[i_].prev_idx = j_;
                        ans[i_].state = EExpected::Greater;
                    }
                }
                if (ans[j_].state == EExpected::Less) {
                    break;
                }
            case EExpected::Greater:
                if (seq[j_] < seq[i_] && ans[i_].length < ans[j_].length + 1) {
                    ans[i_] = { ans[j_].length + 1, static_cast<ssize_t>(j_), EExpected::Less };
                } else if (seq[j_] < seq[i_] && ans[i_].length == ans[j_].length + 1) {
                    std::vector<size_t> cur_seq = get_indices_sequence(ans, i_);
                    std::vector<size_t> potentially_seq = get_indices_sequence(ans, j_);
                    potentially_seq.push_back(i_);
                    if (potentially_seq < cur_seq) {
                        ans[i_].prev_idx = j_;
                        ans[i_].state = EExpected::Less;
                    }
                }
            }
        }
    }
    return ans;
}

std::vector<size_t> get_max_elems_indices(const std::vector<DynamicItem> &dyn_res) {
    std::vector<size_t> res;
    size_t max_length = std::max_element(dyn_res.begin(), dyn_res.end())->length;
    for (size_t i = 0; i < dyn_res.size(); ++i) {
        if (dyn_res[i].length == max_length) {
            res.push_back(i);
        }
    }
    return res;
}

int main() {
    size_t seq_len;
    std::cin >> seq_len;
    std::vector<int> sequence(seq_len);
    for (int &item : sequence) {
        std::cin >> item;
    }

    std::vector<DynamicItem> dyn_res = dynamic_solution(sequence);
    std::vector<size_t> max_idxs = get_max_elems_indices(dyn_res);
    std::vector<std::vector<size_t>> max_seqs;
    max_seqs.reserve(max_idxs.size());
    std::transform(max_idxs.begin(), max_idxs.end(), std::back_inserter(max_seqs),
        [&dyn_res] (size_t idx) { return get_indices_sequence(dyn_res, idx); }
    );
    std::partial_sort(max_seqs.begin(), max_seqs.begin() + 1, max_seqs.end());

    for (size_t idx : max_seqs.front()) {
        std::cout << sequence[idx] << ' ';
    }
    std::cout << std::endl;

    return 0;
}