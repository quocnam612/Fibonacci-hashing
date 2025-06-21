#include "Hash Table/HashTable.hpp"
#include "Helper Function/HelperFunction.hpp"

/*      -- Test Different Key Patterns --
- Input/*.txt contains pairs of unsigned integer KEYS(IDs) and string VALUES(Passwords) for each lines.
    "Source/Input/Random.txt": Random keys pattern.
    "Source/Input/Sequential.txt": Sequential keys pattern.
    "Source/Input/Clustered.txt": Clustered keys pattern.
*/

const std::string INPUT_FILE = "Source/Input/Clustered.txt";
const unsigned int INITIAL_SIZE = 1024; // Initial table size MUST be 2^n
const unsigned int LINE_TO_READ = 500000; // The number of items to read from the input file
const bool USE_FIBONACCI = true; // Use Fibonacci hashing if true, modulo hashing if false
const unsigned int OPERATION_SIZE = LINE_TO_READ / 10; // Number of Items to insert, remove, and search in the hash table
const unsigned int SEED = 3; // Seed for random number generation

int main() {
    srand(SEED);
    LinearHashTable linearHashTable(INITIAL_SIZE, USE_FIBONACCI);
    ChainHashTable chainHashTable(INITIAL_SIZE, USE_FIBONACCI);

// Linear Hash Table Test
    // debugHashTable(INPUT_FILE, linearHashTable, LINE_TO_READ, OPERATION_SIZE);
    testPerformance(INPUT_FILE, linearHashTable, LINE_TO_READ, OPERATION_SIZE);

// Chain Hash Table Test
    // debugHashTable(INPUT_FILE, chainHashTable, LINE_TO_READ, OPERATION_SIZE);
    testPerformance(INPUT_FILE, chainHashTable, LINE_TO_READ, OPERATION_SIZE);

// Test generating different key patterns
    // std::ofstream outputFile("Source/output.txt");
    // std::vector<unsigned int> randomKeys = generateRandomKeys(OPERATION_SIZE);
    // std::vector<unsigned int> sequentialKeys = generateSequentialKeys(OPERATION_SIZE);
    // std::vector<unsigned int> clusteredKeys = generateClusteredKeys(OPERATION_SIZE);
    // if (outputFile.is_open()) for (const int key : clusteredKeys) outputFile << key << std::endl;

    return 0;
}