#pragma once
#include <cstdlib>
#include <iostream>
#include <vector>
#include <chrono>
#include <utility>
#include <windows.h> 
#include <Psapi.h>

std::vector<unsigned int> generateRandomKeys(unsigned int n, unsigned int min = 99999999, unsigned int max = 999999999);
std::vector<unsigned int> generateSequentialKeys(unsigned int n, unsigned int start = 100000000);
std::vector<unsigned int> generateClusteredKeys(unsigned int n, unsigned int start = 100000000);
void printPerformance(float time, float memory);

template <typename Func, typename... Args>
std::pair<double, double> measureExecutionTime(Func func, Args&&... args) { // pair of <Time, Memory>
    PROCESS_MEMORY_COUNTERS memBefore{}, memAfter{};
    auto start = std::chrono::high_resolution_clock::now();
    GetProcessMemoryInfo(GetCurrentProcess(), &memBefore, sizeof(memBefore));

    func(std::forward<Args>(args)...);
    GetProcessMemoryInfo(GetCurrentProcess(), &memAfter, sizeof(memAfter));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    return {
        duration.count() / 1000.0, // Convert microseconds to milliseconds
        static_cast<double>(memAfter.PeakWorkingSetSize - memBefore.PeakWorkingSetSize) / (1024 * 1024) // Convert bytes to MB
    };
}

// Debug hash table contents and the performance function
template <typename HashTableType>
void debugHashTable(const std::string& inputFile, HashTableType& hashTable, unsigned int n, unsigned int opSize) { // !LOW N VALUE!
    std::vector<unsigned int> testKeys;
    std::pair<double, double> timeMemory;
    
    if (inputFile.find("Cluster") != std::string::npos) testKeys = generateClusteredKeys(opSize);
    else if (inputFile.find("Sequential") != std::string::npos) testKeys = generateSequentialKeys(opSize);
    else testKeys = generateRandomKeys(opSize);

    std::cout << "Debugging with " << n << " items from " << inputFile << "\n";

    std::cout << "\n\tInputing " << n << " items...\n";
    timeMemory = measureExecutionTime([&]() {
        hashTable.input(inputFile, n);
    });
    hashTable.output(hashTable.size);
    printPerformance(timeMemory.first, timeMemory.second);
    hashTable.resetCollisions();

    std::cout << "\n\tAdding " << opSize << " items...\n";
    timeMemory = measureExecutionTime([&]() {
        for (unsigned int key : testKeys) {
            hashTable.insert(key, "Password123@");
        }
    });
    hashTable.output(hashTable.size);
    printPerformance(timeMemory.first, timeMemory.second);
    hashTable.resetCollisions();

    std::cout << "\n\tSearching " << opSize << " items...\n";
    timeMemory = measureExecutionTime([&]() {
        for (unsigned int key : testKeys) {
            hashTable.search(key);
        }
    });
    hashTable.output(hashTable.size);
    printPerformance(timeMemory.first, timeMemory.second);
    hashTable.resetCollisions();

    std::cout << "\n\tRemoving " << opSize << " items...\n";
    timeMemory = measureExecutionTime([&]() {
        for (unsigned int key : testKeys) {
            hashTable.remove(key);
        }
    });
    hashTable.output(hashTable.size);
    printPerformance(timeMemory.first, timeMemory.second);
}

// Print statistics and performance of the hash table
template <typename HashTableType>
void testPerformance(const std::string& inputFile, HashTableType& hashTable, unsigned int n, unsigned int opSize) {
    std::vector<unsigned int> testKeys = generateRandomKeys(opSize);
    std::pair<double, double> timeMemory;

    std::cout << "Testing performance with " << n << " items from " << inputFile << "\n";

    std::cout << "\n\tInputing " << n << " items...\n";
    timeMemory = measureExecutionTime([&]() {
        hashTable.input(inputFile, n);
    });    
    hashTable.outputStats();
    printPerformance(timeMemory.first, timeMemory.second);
    hashTable.resetCollisions();

    std::cout << "\n\tAdding " << opSize << " items...\n";
    timeMemory = measureExecutionTime([&]() {
        for (unsigned int key : testKeys) {
            hashTable.insert(key, "Password123@");
        }
    });
    hashTable.outputStats();
    printPerformance(timeMemory.first, timeMemory.second);
    hashTable.resetCollisions();

    std::cout << "\n\tSearching " << opSize << " items...\n";
    timeMemory = measureExecutionTime([&]() {
        for (unsigned int key : testKeys) {
            hashTable.search(key);
        }
    });
    hashTable.outputStats();
    printPerformance(timeMemory.first, timeMemory.second);
    hashTable.resetCollisions();

    std::cout << "\n\tRemoving " << opSize << " items...\n";
    timeMemory = measureExecutionTime([&]() {
        for (unsigned int key : testKeys) {
            hashTable.remove(key);
        }
    });
    hashTable.outputStats();
    printPerformance(timeMemory.first, timeMemory.second);
}