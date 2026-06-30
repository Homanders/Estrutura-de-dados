#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <cstddef>
#include "ListNode.h"

class LinkedList
{
private:
    ListNode* head;
    ListNode* tail;
    int elementCount;

public:
    LinkedList();
    ~LinkedList();

    void clear();
    void pushFront(const SensorData& sensor);
    void pushBack(const SensorData& sensor);
    void printList() const;
    bool contains(long timestamp) const;
    SensorData* find(long timestamp);
    bool remove(long timestamp);
    SensorData* front();
    SensorData* back();
    bool empty() const;
    void printStatistics() const;
    void printAnalysis() const;
    bool popFront();
    bool popBack();
    void reverse();
    int size() const;
    std::size_t memoryUsage() const;
};

#endif
