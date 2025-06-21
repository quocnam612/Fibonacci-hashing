#include "HashTable.hpp"
        /*--Hash methods--*/
unsigned int moduloHash(const unsigned int& key, const unsigned int& size) {
    return key & (size - 1); // Optimized modulo for power of 2 sizes
}

unsigned int fibonacciHash(const unsigned int& key, const unsigned int& n) {
    return (key * 2654435769U) >> (32 - n); // Optimized Fibonacci hashing for power of 2 sizes, key type is unsigned integer(4 bytes - 32 bits)
}

        /*--Open addressing--*/
LinearHashTable::LinearHashTable(unsigned int size, bool useFibonacci) : useFibonacci(useFibonacci), size(size), sizePower(log2(size)), table(size), collisions(size) {}

void LinearHashTable::resetCollisions() {
    collisions.assign(size, std::nullopt); // Reset collision counts
}

unsigned int LinearHashTable::hashFunction(const unsigned int& key) {
    if (useFibonacci) return fibonacciHash(key, sizePower);
    return moduloHash(key, size);
}

void LinearHashTable::insert(unsigned int key, std::string value) {
    float loadFactor = count / float(size);
    if (loadFactor > 0.7) resize(size * 2); // Resize if load factor exceeds 0.7
    
    unsigned int index = hashFunction(key);
    unsigned int originalIndex = index; // Store original index for collision tracking
    while(table[index]) {
        if (table[index].value().key == key) {
            table[index].value().value = value; // Update value if key already exists
            return;
        }
        index = (index + 1) % size; // Linear probing
        if(collisions[originalIndex]) collisions[originalIndex].value()++; // Track collisions
        else collisions[originalIndex] = 1; // Initialize collision count if not set
    }
    table[index] = Node{key, value}; // Insert new key-value pair
    count++;
}

void LinearHashTable::remove(unsigned int key) {
    unsigned int index = hashFunction(key);
    unsigned int originalIndex = index; // Store original index for collision tracking
    while(table[index]) {
        if (table[index].value().key == key) { // Found
            table[index] = std::nullopt; // Remove the key
            count--;
            return;
        }
        index = (index + 1) % size; // Linear probing
        if(collisions[originalIndex]) collisions[originalIndex].value()++; // Track collisions
        else collisions[originalIndex] = 1; // Initialize collision count if not set
    }
}

std::string LinearHashTable::search(unsigned int key) {
    unsigned int index = hashFunction(key);
    unsigned int originalIndex = index; // Store original index for collision tracking
    while(table[index]) {
        if (table[index].value().key == key) return table[index].value().value; // Found
        index = (index + 1) % size; // Linear probing
        if(collisions[originalIndex]) collisions[originalIndex].value()++; // Track collisions
        else collisions[originalIndex] = 1; // Initialize collision count if not set
    }
    return ""; // Not found
}

void LinearHashTable::resize(unsigned int newSize) {
    size = newSize;
    sizePower = log2(newSize);
    std::vector<std::optional<Node>> oldTable = std::move(table);
    table = std::vector<std::optional<Node>>(newSize);
    collisions.resize(newSize);
    
    for (auto& item : oldTable) {
        if (item) {
            unsigned int index = hashFunction(item.value().key);
            unsigned int originalIndex = index; // Store original index for collision tracking
            while (table[index]) {
                index = (index + 1) % size; // Linear probing
                if(collisions[originalIndex]) collisions[originalIndex].value()++; // Track collisions
                else collisions[originalIndex] = 1; // Initialize collision count if not set
            }
            table[index] = item; // Reinsert the item
        }
    }
}

void LinearHashTable::input(const std::string& path, unsigned int n) {
    std::ifstream file(path);
    if (file.is_open()) {
        unsigned int key;
        std::string value;
        while (file >> key >> value && n-- > 0) {
            insert(key, value);
        }
        file.close();
    }
}

void LinearHashTable::outputStats() {
    unsigned int maxCollisions = 0, totalCollisions = 0;
    for (unsigned int i = 0; i < size; i++) {
        if (collisions[i]) {
            maxCollisions = std::max(maxCollisions, collisions[i].value());
            totalCollisions += collisions[i].value();
        }
    }

    std::cout << "\t\t---" << (useFibonacci ? "Fibonacci" : "Modulo") << " Linear Hash Table---\n"
              << "\tSize: " << size << "\tNumber of items: " << count << "\tLoad Factor: " << count / float(size) << std::endl;
    std::cout << "\n-Max Collisions: " << maxCollisions 
              << "\n-Total Collisions: " << totalCollisions
              << "\n-Average Collisions: " << totalCollisions / float(size) << std::endl;
}

void LinearHashTable::output(unsigned int n) {
    unsigned int maxCollisions = 0, totalCollisions = 0;
    std::cout << "\t\t---" << (useFibonacci ? "Fibonacci" : "Modulo") << " Linear Hash Table---\n"
              << "\tSize: " << size << "\tNumber of items: " << count << "\tLoad Factor: " << count / float(size) << std::endl;

    for (unsigned int i = 0; i < n; i++) {
        std::cout << std::endl << i << ": ";
        if (table[i]) std::cout << table[i].value().key << "|" << table[i].value().value;
        else std::cout << "null";
        if (collisions[i]) {
            std::cout << "\n\tCollisions: " << collisions[i].value();
            maxCollisions = std::max(maxCollisions, collisions[i].value());
            totalCollisions += collisions[i].value();
        }
    }
    std::cout << "\n-Max Collisions: " << maxCollisions 
              << "\n-Total Collisions: " << totalCollisions
              << "\n-Average Collisions: " << totalCollisions / float(size) << std::endl;
}

        /*--Closed addressing--*/
ChainHashTable::ChainHashTable(unsigned int size, bool useFibonacci) : useFibonacci(useFibonacci), size(size), sizePower(log2(size)), table(size), collisions(size) {}

ChainHashTable::~ChainHashTable() {
    for (Node* head : table) {
        Node* current = head;
        while (current) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete; // Free memory
        }
    }
}

void ChainHashTable::resetCollisions() {
    collisions.assign(size, std::nullopt); // Reset collision counts
}

unsigned int ChainHashTable::hashFunction(const unsigned int& key) {
    if (useFibonacci) return fibonacciHash(key, sizePower);
    return moduloHash(key, size);
}

void ChainHashTable::insert(unsigned int key, std::string value) {
    float loadFactor = count / float(size);
    if (loadFactor > 2.0) resize(size * 2);

    unsigned int index = hashFunction(key);
    Node* current = table[index];
    while(current) { // Check for alreay existing key
        if (current->key == key) {
            current->value = value; // Update value if key already exists
            return;
        }
        current = current->next;
        if(collisions[index]) collisions[index].value()++; // Track collisions
        else collisions[index] = 1; // Initialize collision count if not set
    }
    Node* newNode = new Node{key, value, table[index]};
    table[index] = newNode;
    count++;
}

void ChainHashTable::remove(unsigned int key) {
    unsigned int index = hashFunction(key);
    Node* current = table[index];
    Node* prev = nullptr;
    while(current) {
        if (current && current->key == key) {
            if (prev) prev->next = current->next; // Remove from chain
            else table[index] = current->next; // Remove from head
            delete current;
            count--;
            return;
        }
        prev = current;
        current = current->next;
        if(collisions[index]) collisions[index].value()++; // Track collisions
        else collisions[index] = 1; // Initialize collision count if not set
    }
}

std::string ChainHashTable::search(unsigned int key) {
    unsigned int index = hashFunction(key);
    Node* current = table[index];
    while(current) {
        if ( current->key == key) return current->value; // Found
        current = current->next;
        if(collisions[index]) collisions[index].value()++; // Track collisions
        else collisions[index] = 1; // Initialize collision count if not set
    }
    return ""; // Not found
}

void ChainHashTable::resize(unsigned int newSize) {
    std::vector<Node*> oldTable = table;
    table.assign(newSize, nullptr);
    collisions.resize(newSize);
    sizePower = log2(newSize);
    size = newSize;

    for (Node* head : oldTable) {
        Node* current = head;
        while (current) {
            unsigned int index = hashFunction(current->key);
            Node* newNode = new Node{current->key, current->value, table[index]};
            table[index] = newNode;

            Node* toDelete = current;
            current = current->next;
            if(collisions[index]) collisions[index].value()++; // Track collisions
            else collisions[index] = 1; // Initialize collision count if not set
            delete toDelete;
        }
    }
}

void ChainHashTable::input(const std::string& path, unsigned int n) {
    std::ifstream file(path);
    if (file.is_open()) {
        unsigned int key;
        std::string value;
        while (file >> key >> value && n-- > 0) insert(key, value);
        file.close();
    }
}

void ChainHashTable::outputStats() {
    unsigned int maxCollisions = 0, totalCollisions = 0;
    for (unsigned int i = 0; i < size; i++) {
        if (collisions[i]) {
            maxCollisions = std::max(maxCollisions, collisions[i].value());
            totalCollisions += collisions[i].value();
        }
    }

    std::cout << "\t\t---" << (useFibonacci ? "Fibonacci" : "Modulo") << " Chain Hash Table---\n"
              << "\tSize: " << size << "\tNumber of items: " << count << "\tLoad Factor: " << count / float(size) << std::endl;
    std::cout << "\n-Max Collisions: " << maxCollisions 
              << "\n-Total Collisions: " << totalCollisions
              << "\n-Average Collisions: " << totalCollisions / float(size) << std::endl;
}

void ChainHashTable::output(unsigned int n) {
    unsigned int maxCollisions = 0, totalCollisions = 0;
    std::cout << "\t\t---" << (useFibonacci ? "Fibonacci" : "Modulo") << " Chain Hash Table---\n"
              << "\tSize: " << size << "\tNumber of items: " << count << "\tLoad Factor: " << count / float(size) << std::endl;

    for (unsigned int i = 0; i < n; i++) {
        std::cout << std::endl << i << ": ";

        if (table[i]) {
            std::cout << table[i]->key << "|" << table[i]->value;
            Node* current = table[i]->next;
            while (current) {
                std::cout << " -> " << current->key << "|" << current->value;
                current = current->next;
            }
        }
        else std::cout << "null";
        if (collisions[i]) {
            std::cout << "\n\tCollisions: " << collisions[i].value();
            maxCollisions = std::max(maxCollisions, collisions[i].value());
            totalCollisions += collisions[i].value();
        }
    }
    std::cout << "\n-Max Collisions: " << maxCollisions 
              << "\n-Total Collisions: " << totalCollisions
              << "\n-Average Collisions: " << totalCollisions / float(size) << std::endl;
}