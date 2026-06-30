#ifndef LISTNODE_H
#define LISTNODE_H

#include "SensorData.h"

class ListNode
{
public:

    SensorData data;

    ListNode* next;

    ListNode(const SensorData& sensor)
    {
        data = sensor;
        next = nullptr;
    }
};

#endif