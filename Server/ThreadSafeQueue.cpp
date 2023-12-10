#include "ThreadSafeQueue.hpp"

// Initializes a thread-safe queue
void InitializeQueue(Queue* queue) {
    queue->head = nullptr;
    queue->tail = nullptr;
    InitializeCriticalSection(&queue->lock); // Initialize critical section
}

// Enqueues a MeasurementData element into the thread-safe queue
void Enqueue(Queue* queue, MeasurementData* newData) {
    Node* newNode = new Node();
    newNode->data = newData;
    newNode->next = nullptr;

    EnterCriticalSection(&queue->lock); // Enter critical section

    if (queue->head == nullptr) {
        queue->head = newNode;
        queue->tail = newNode;
    }
    else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }

    LeaveCriticalSection(&queue->lock); // Leave critical section
}

// Dequeues a MeasurementData element from the thread-safe queue
MeasurementData* Dequeue(Queue* queue) {
    EnterCriticalSection(&queue->lock); // Enter critical section

    if (queue->head == nullptr) {
        LeaveCriticalSection(&queue->lock); // Leave critical section
        return nullptr;
    }

    Node* temp = queue->head;
    MeasurementData* data = temp->data;

    queue->head = queue->head->next;
    delete temp;

    LeaveCriticalSection(&queue->lock); // Leave critical section

    return data;
}

// Clears the thread-safe queue and frees allocated memory
void ClearQueue(Queue* queue) {
    EnterCriticalSection(&queue->lock); // Enter critical section

    while (queue->head != nullptr) {
        Node* temp = queue->head;
        queue->head = queue->head->next;
        delete temp->data;
        delete temp;
    }

    queue->tail = nullptr;

    LeaveCriticalSection(&queue->lock); // Leave critical section
}

// Destroys the thread-safe queue and releases allocated resources
void DestroyQueue(Queue* queue) {
    ClearQueue(queue);
    DeleteCriticalSection(&queue->lock); // Delete critical section
}
