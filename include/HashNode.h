#ifndef HASHNODE_H
#define HASHNODE_H

#include "SensorData.h"

struct HashNode
{
    SensorData data;

    HashNode* next;

    HashNode(const SensorData& sensor)
    {
        data = sensor;
        next = nullptr;
    }
};

#endif