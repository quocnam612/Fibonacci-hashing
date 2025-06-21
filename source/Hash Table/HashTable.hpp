#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <optional>
#include <cmath>

unsigned int moduloHash(const unsigned int& key, const unsigned int& size);
unsigned int fibonacciHash(const unsigned int& key, const unsigned int& n); // size is 2^n

//          --Open addressing--
// Linear probing (resize when load factor > 0.7)
struct LinearHashTable {
    struct Node {
        unsigned int key; // UserID
        std::string value; // Password
    };
    bool useFibonacci; // Use Fibonacci hashing if true, modulo hashing if false
    unsigned int size, sizePower, count = 0;
    std::vector<std::optional<Node>> table;
    std::vector<std::optional<unsigned int>> collisions; // Use to track max and average collision counts

    LinearHashTable(unsigned int size, bool useFibonacci = false);

    void resetCollisions(); // Reset collision counts
    unsigned int hashFunction(const unsigned int& key); // Hashing method based on useFibonacci flag
    void insert(unsigned int key, std::string value); // Insert if not found key or update value if found key
    void remove(unsigned int key); // Remove if found key or do nothing if not found
    std::string search(unsigned int key); // Return value if found, "" if not found
    void resize(unsigned int newSize);
    void input(const std::string& path, unsigned int n); // Only read the first n lines from the file
    void outputStats(); // Output statistics of the hash table
    void output(unsigned int n); // Output n items of the hash table
};


//          --Closed addressing--
// Separate chaining (resize when load factor > 2)
struct ChainHashTable {
    struct Node {
        unsigned int key; // UserID
        std::string value; // Password
        Node* next = nullptr;
    };
    bool useFibonacci; // Use Fibonacci hashing if true, modulo hashing if false
    unsigned int size, sizePower, count = 0;
    std::vector<Node*> table;
    std::vector<std::optional<unsigned int>> collisions; // Use to track max and average collisions

    ChainHashTable(unsigned int size, bool useFibonacci = false);
    ~ChainHashTable(); // Destructor to vector of pounsigned inters Nodes

    void resetCollisions(); // Reset collision counts
    unsigned int hashFunction(const unsigned int& key); // Hashing method based on useFibonacci flag
    void insert(unsigned int key, std::string value); // Insert if not found key or update value if found key
    void remove(unsigned int key); // Remove if found key or do nothing if not found
    std::string search(unsigned int key); // Return value if found, "" if not found
    void resize(unsigned int newSize);
    void input(const std::string& path, unsigned int n); // Only read the first n lines from the file
    void outputStats(); // Output statistics of the hash table
    void output(unsigned int n); // Output n items of the hash table
};