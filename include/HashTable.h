#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cstddef>
#include <iostream>
#include "HashNode.h"

using namespace std;

class HashTable
{
private:
    HashNode** table;
    int tableSize;
    int elementCount;
    int collisionCount;
    int usedBuckets;

public:
    HashTable(int size = 101);
    ~HashTable();

    void clear();
    int size() const;
    int capacity() const;
    int hash(long timestamp) const;
    void insert(const SensorData& sensor);
    bool remove(long timestamp);

    int collisions() const;
    double collisionRate() const;
    std::size_t memoryUsage() const;

    int bucketsUsed() const;
    double loadFactor() const;
    bool contains(long timestamp) const;
    SensorData* find(long timestamp);
    void printTable() const;
    int emptyBuckets() const;
    int maxChainLength() const;
    int minChainLength() const;
    double averageChainLength() const;
    double occupancyRate() const;
    void printAnalysis() const;
};

#endif
