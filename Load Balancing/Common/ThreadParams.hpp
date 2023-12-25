#ifndef THREAD_PARAMS_HPP
#define THREAD_PARAMS_HPP

#include <winsock2.h>
#include "../Server/ThreadSafeQueue.hpp"

/**
 * @struct ThreadParams
 * @brief Structure containing parameters for a thread.
 */
typedef struct ThreadParams {
    SOCKET clientSocket; /**< Socket for client communication */
    Queue* queue; /**< Pointer to the queue for task management */
    int threadId; /**< Thread identifier */
    bool* threadPoolClientsStatus; /**< Pointer to array indicating client thread status */
    CRITICAL_SECTION cs; /**< Critical section for synchronization */
} ThreadParams;

#endif // THREAD_PARAMS_HPP
