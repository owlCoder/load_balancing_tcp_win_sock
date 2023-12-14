#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include "../Common/Queue.hpp"

/**
 * @brief Initializes a thread-safe queue.
 *
 * This function initializes the provided Queue struct
 * by setting its head and tail pointers to NULL and initializing
 * the critical section for synchronization.
 *
 * @param queue A pointer to the Queue to be initialized.
 */
void InitializeQueue(Queue* queue);

/**
 * @brief Enqueues a MeasurementData element into the thread-safe queue.
 *
 * Adds a new MeasurementData element to the end of the thread-safe queue.
 *
 * @param queue A pointer to the Queue.
 * @param newData A pointer to the MeasurementData struct to be enqueued.
 */
void Enqueue(Queue* queue, MeasurementData* newData);

/**
 * @brief Dequeues a MeasurementData element from the thread-safe queue.
 *
 * Removes and returns the MeasurementData element from the front of the queue.
 * Returns NULL if the queue is empty.
 *
 * @param queue A pointer to the Queue.
 * @return A pointer to the dequeued MeasurementData element, or NULL if the queue is empty.
 */
struct MeasurementData* Dequeue(Queue* queue);

/**
 * @brief Clears the thread-safe queue and frees allocated memory.
 *
 * Removes all elements from the queue and frees the memory occupied by the elements.
 * After calling this function, the queue will be empty.
 *
 * @param queue A pointer to the Queue to be cleared.
 */
void ClearQueue(Queue* queue);

/**
 * @brief Destroys the thread-safe queue and releases allocated resources.
 *
 * Removes all elements from the queue, frees the memory occupied by the elements,
 * and destroys the critical section used for synchronization.
 *
 * @param queue A pointer to the Queue to be destroyed.
 */
void DestroyQueue(Queue* queue);

/**
 * @brief Retrieves the current size of the thread-safe queue.
 *
 * This function returns the number of elements present in the thread-safe queue without
 * traversing through the entire queue. The queue size is maintained and updated whenever
 * elements are enqueued, allowing for efficient retrieval of the current queue size.
 *
 * @param queue A pointer to the thread-safe queue.
 * @param dataProcessed A number how much data was processed in time interval
 * @return An integer representing the current size of the queue.
 */
int QueueSize(Queue* queue, int dataProcessed);

/**
 * @brief Initiates a graceful shutdown process for the application.
 *
 * This function triggers a graceful shutdown process in the application, allowing it to clean up resources,
 * stop accepting new tasks or connections, and complete any ongoing operations before exiting.
 *
 * @param queue A pointer to the queue structure used in the application.
 * @return An integer indicating the status of the shutdown process.
 *         Returns 0 upon successful initiation of shutdown; otherwise, returns a non-zero value to indicate an error.
 */
int ShutDown(Queue* queue);

/**
 * @brief Sets the shutdown flag in the application.
 *
 * This function sets an internal flag or variable in the application, indicating that a shutdown has been requested.
 * It is often used in combination with a shutdown process, enabling controlled termination or cleanup activities.
 *
 * @param queue A pointer to the queue structure used in the application.
 */
void SetShutDown(Queue* queue);

#endif /* THREAD_SAFE_QUEUE_HPP */