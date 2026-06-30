#ifndef AVL_H
#define AVL_H

#include "AVLNode.h"
#include <cstddef>
#include <iostream>

class AVL
{
private:
    AVLNode* root;
    int nodeCount;
    int maxSize;

    int height(AVLNode* node);
    void updateHeight(AVLNode* node);
    int getBalance(AVLNode* node);

    AVLNode* leftRotate(AVLNode* node);
    AVLNode* rightRotate(AVLNode* node);
    AVLNode* balance(AVLNode* node);

    AVLNode* insert(AVLNode* node, const SensorData& sensor);
    AVLNode* remove(AVLNode* node, long timestamp);
    AVLNode* findMin(AVLNode* node);
    AVLNode* search(AVLNode* node, long timestamp);

    void inOrder(AVLNode* node);
    void preOrder(AVLNode* node);
    void postOrder(AVLNode* node);
    void destroy(AVLNode* node);

public:
    AVL();
    ~AVL();

    void insert(const SensorData& sensor);
    void remove(long timestamp);

    bool contains(long timestamp);
    SensorData* find(long timestamp);

    bool isEmpty();
    int size();
    int height();

    void setMaxSize(int max);
    int getMaxSize() const;
    std::size_t memoryUsage() const;

    void printInOrder();
    void printPreOrder();
    void printPostOrder();

    void clear();
};

#endif
