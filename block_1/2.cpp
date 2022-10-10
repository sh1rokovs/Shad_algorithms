#include <iostream>
#include <vector>
#include <string>
#include <stack>

int main() {
    std::stack<char> st;
    std::string ltBrackets = "{([";
    std::string rtBrackets = "})]";

    std::string task;
    std::cin >> task;

    if (task.size() <= 1000000) {
        for (unsigned int iter = 0; iter < task.size(); ++iter) {
            if (task[iter] == '{' || task[iter] == '[' || task[iter] == '(') {
                st.push(task[iter]);
                continue;
            }

            if (task[iter] == '}' || task[iter] == ']' || task[iter] == ')') {
                if (st.size() == 0) {
                    std::cout << iter;
                    return 0;
                }

                char pop = st.top();
                st.pop(); 
                if (ltBrackets.find(pop, 0) != rtBrackets.find(task[iter], 0)) {
                    std::cout << iter;
                    return 0;
                }
            }
        }

        if (st.size() == 0) {
            std::cout << "CORRECT";
        } else {
            std::cout << task.size();
        }
    }

    return 0;
}