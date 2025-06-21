#include "HelperFunction.hpp"

std::vector<unsigned int> generateRandomKeys(unsigned int n, unsigned int min, unsigned int max) {
    std::vector<unsigned int> keys;
    for (unsigned int i = 0; i < n; i++) {
        keys.push_back(rand() % (max - min + 1) + min);
    }
    return keys;
}

std::vector<unsigned int> generateSequentialKeys(unsigned int n, unsigned int start) {
    std::vector<unsigned int> keys;
    for (unsigned int i = 0; i < n; i++) {
        keys.push_back(start + i);
    }
    return keys;
}

std::vector<unsigned int> generateClusteredKeys(unsigned int n, unsigned int start, unsigned int clusterSize, unsigned int clusterGap) {
    std::vector<unsigned int> keys;
    keys.reserve(n);
    unsigned int currentBase = start;

    while (keys.size() < n) {
        for (unsigned int i = 0; i < clusterSize && keys.size() < n; ++i) {
            keys.push_back(currentBase + i);
        }
        currentBase += clusterGap;
    }

    return keys;
}

void printPerformance(float time, float memory) {
    std::cout << "Execution Time: " << time << " ms\n";
    std::cout << "Memory Usage: " << memory << " MB\n";
}