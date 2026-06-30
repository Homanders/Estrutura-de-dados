#include "SkipNode.h"

SkipNode::SkipNode(const SensorData& sensor, int nodeLevel)
{
    data = sensor;

    level = nodeLevel;

    forward = new SkipNode*[level + 1];

    for(int i = 0; i <= level; i++)
    {
        forward[i] = nullptr;
    }
}

SkipNode::~SkipNode()
{
    delete[] forward;
}