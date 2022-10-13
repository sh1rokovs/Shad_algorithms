#include <iostream>
#include <string>
#include <vector>
#include <deque>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    size_t array_size, num_requests;
    std::cin >> array_size;
    std::vector<int> array(array_size);
    for (int &item : array) {
        std::cin >> item;
    }

    std::cin >> num_requests;
    std::deque<int> maximum_sequence(array.begin(), array.begin() + 1);
    for (size_t i_ = 0, lhs = 0, rhs = 0; i_ < num_requests; ++i_) {
        std::string request;
        std::cin >> request;
        if (request == "L") {
            if (!maximum_sequence.empty() && maximum_sequence.front() == array[lhs]) {
                maximum_sequence.pop_front();
            }
            ++lhs;
        } else if (request == "R") {
            ++rhs;
            while (!maximum_sequence.empty() && maximum_sequence.back() < array[rhs]) {
                maximum_sequence.pop_back();
            }
            maximum_sequence.push_back(array[rhs]);
        }
        std::cout << maximum_sequence.front() << ' ';
    }
    std::cout << std::endl;

    return 0;
}