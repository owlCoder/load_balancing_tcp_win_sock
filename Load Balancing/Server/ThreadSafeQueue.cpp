#include "ThreadSafeQueue.hpp"
#include "../Common/Logger.hpp"

#pragma region INITIALIZE QUEUE
// Initializes a thread-safe queue
void InitializeQueue(Queue* queue) {
    queue->head = nullptr;
    queue->tail = nullptr;
    queue->size = 0; // Initialize size to zero
    queue->total = 0; // Initialize total enqueued data to zero
    queue->shutdown = 0;
    InitializeCriticalSection(&queue->lock); // Initialize critical section
}
#pragma endregion

#pragma region ENQUEUE
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

    queue->size++; // Increment size when enqueueing new data
    queue->total++; // Increment total data size put in queue

    LeaveCriticalSection(&queue->lock); // Leave critical section
}
#pragma endregion

#pragma region DEQUEUE
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

    queue->size--; // Decrement size when dequeuing data

    LeaveCriticalSection(&queue->lock); // Leave critical section

    return data;
}
#pragma endregion

#pragma region TOTAL IN QUEUE
// Get the total size of queue
long int QueueTotalDataEnqueued(Queue* queue) {
    EnterCriticalSection(&queue->lock); // Enter critical section
    int total = queue->total;
    LeaveCriticalSection(&queue->lock); // Leave critical section
    return total;
}
#pragma endregion

#pragma region QUEUE SIZE
// Get the current size of the queue
int QueueSize(Queue* queue) {
    EnterCriticalSection(&queue->lock); // Enter critical section
    int size = queue->size;
    LeaveCriticalSection(&queue->lock); // Leave critical section
    return size;
}
#pragma endregion

#pragma region SHUTDOWN FLAG
// Return shutdown flag state
int ShutDown(Queue* queue) {
    EnterCriticalSection(&queue->lock); // Enter critical section
    int shutdown = queue->shutdown;
    LeaveCriticalSection(&queue->lock); // Leave critical section
    return shutdown;
}

// Set shutdown flag state
void SetShutDown(Queue* queue)
{
    EnterCriticalSection(&queue->lock); // Enter critical section
    queue->shutdown = 1;
    LeaveCriticalSection(&queue->lock); // Leave critical section
}
#pragma endregion

#pragma region CLEAR QUEUE
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
    queue->size = 0; // Reset size to zero after clearing the queue

    LeaveCriticalSection(&queue->lock); // Leave critical section
}
#pragma endregion

#pragma region DESTROY QUEUE
// Destroys the thread-safe queue and releases allocated resources
void DestroyQueue(Queue* queue) {
    ClearQueue(queue);
    DeleteCriticalSection(&queue->lock); // Delete critical section
}
#pragma endregion