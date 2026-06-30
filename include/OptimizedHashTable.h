#ifndef OPTIMIZEDHASHTABLE_H
#define OPTIMIZEDHASHTABLE_H

#include <cstddef>
#include "SensorData.h"

class OptimizedHashTable
{
private:
    struct OptimizedHashNode
    {
        SensorData data;
        OptimizedHashNode* next;

        OptimizedHashNode(const SensorData& sensor)
        {
            data = sensor;
            next = nullptr;
        }
    };

    OptimizedHashNode** table;
    int tableSize;
    int elementCount;
    int usedBuckets;
    long long collisionCount;
    long long rehashCount;
    double maxLoadFactor;

    int hash(long long timestamp) const;
    bool isPrime(int value) const;
    int nextPrime(int value) const;
    void allocateTable(int size);
    void rehash();
    void insertRehashed(const SensorData& sensor);

public:
    OptimizedHashTable(int initialSize = 101, double maxLoad = 0.75);
    ~OptimizedHashTable();

    void clear();
    void insert(const SensorData& sensor);
    bool remove(long long timestamp);
    bool contains(long long timestamp) const;
    SensorData* find(long long timestamp);
    bool isEmpty() const;
    int size() const;
    int capacity() const;
    int bucketsUsed() const;
    int emptyBuckets() const;
    long long collisions() const;
    long long rehashes() const;
    double collisionRate() const;
    std::size_t memoryUsage() const;
    double loadFactor() const;
    double occupancyRate() const;
    int maxChainLength() const;
    int minChainLength() const;
    double averageChainLength() const;
    bool validate() const;
    void printAnalysis() const;
};

#endif
