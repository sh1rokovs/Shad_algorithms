#include <iostream>
#include <vector>
#include <algorithm>

int binsearch_for_minmax(std::vector<int> vectorA, std::vector<int> vectorB, int length) {
    int varL = -1;
    int varR = length;
    while (varR - varL > 1) {
        int varM = (varL + varR) / 2;

        if (vectorA[varM] < vectorB[varM]) {
            varL = varM;
        } else {
            varR = varM;
        }
    }

    if (varR == length || varL >= 0) {
        if (std::max(vectorA[varL], vectorB[varL]) < std::max(vectorA[varR], vectorB[varR])) {
            return varL;
        }
    }

    return varR;
}

int main() {
    std::vector<int> result;
    int varN = 0, varM = 0, varL = 0, varQ = 0;

    std::cin >> varN >> varM >> varL;

    std::vector<std::vector<int>> vectorA(varN, std::vector<int>(varL));
    std::vector<std::vector<int>> vectorB(varM, std::vector<int>(varL));

    for (int varI = 0; varI < varN; ++varI) {          
        for (int varJ = 0; varJ < varL; ++varJ) {
            std::cin >> vectorA[varI][varJ];
        }
    }

    for (int varI = 0; varI < varM; ++varI) {         
        for (int varJ = 0; varJ < varL; ++varJ) {
            std::cin >> vectorB[varI][varJ];
        }
    }

    std::cin >> varQ;

    for (int varZ = 0; varZ < varQ; ++varZ) {
        int a_i = 0;
        int b_j = 0;
        std::cin >> a_i >> b_j;
        result.push_back(binsearch_for_minmax(vectorA[a_i - 1], vectorB[b_j - 1], varL) + 1);
    }

    for (int varI = 0; varI < varQ; ++varI) {
        std::cout << result[varI] << std::endl;
    }

    return 0;
}