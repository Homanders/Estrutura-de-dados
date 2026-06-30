#ifndef AVLNODE_H
#define AVLNODE_H

#include "SensorData.h"

struct AVLNode
{
    SensorData data;

    AVLNode* left;
    AVLNode* right;

    int height;

    AVLNode(const SensorData& sensor)
    {
        data = sensor;

        left = nullptr;
        right = nullptr;

        height = 1;
    }
};

#endif