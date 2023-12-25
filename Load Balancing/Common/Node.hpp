#ifndef NODE_HPP
#define NODE_HPP

#include "MeasurementData.hpp"

// Node structure for the queue
typedef struct Node {
    MeasurementData* data;
    Node* next;
} Node;

#endif // NODE_HPP
