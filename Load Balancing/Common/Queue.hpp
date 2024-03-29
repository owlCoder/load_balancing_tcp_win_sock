#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "Node.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h> // For synchronization primitives in Windows

// Define the thread-safe queue
typedef struct Queue {
    Node* head;             // Pointer to the head of the queue
    Node* tail;             // Pointer to the tail of the queue
    CRITICAL_SECTION lock;  // Critical section for synchronization
    int size;               // Size of the queue
    long int total;         // Total size of put elements in the queue
    int shutdown;           // Shutdown flag to stop processing data
} Queue;

#endif // QUEUE_HPP