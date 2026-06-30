#ifndef SKIPNODE_H
#define SKIPNODE_H

#include "SensorData.h"

class SkipNode
{
public:

    SensorData data;

    SkipNode** forward;

    int level;

    SkipNode(const SensorData& sensor, int nodeLevel);

    ~SkipNode();
};

#endif