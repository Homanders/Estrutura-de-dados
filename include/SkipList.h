#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <cstddef>
#include "SkipNode.h"

class SkipList
{
private:
    static const int MAX_LEVEL = 6;
    SkipNode* header;
    int currentLevel;
    int elementCount;
    float probability;

    int randomLevel();

public:
    SkipList();
    ~SkipList();

    void clear();
    void insert(const SensorData& sensor);
    bool contains(long timestamp) const;
    SensorData* find(long timestamp);
    bool remove(long timestamp);
    void searchWithTrace(long timestamp) const;
    void printLevelZero() const;
    void printLevels() const;
    void printStatistics() const;
    bool validateOrder() const;
    bool validateIntegrity() const;
    bool empty() const;
    int size() const;
    int level() const;
    std::size_t memoryUsage() const;
};

#endif
