#ifndef ACCEPT_CLIENT_THREAD_PARAMS_HPP
#define ACCEPT_CLIENT_THREAD_PARAMS_HPP

#include <WinSock2.h>
#include "Queue.hpp"

/**
 * @struct AcceptClientThreadParams
 * @brief Structure to pass data to the AcceptClientConnections thread.
 */
typedef struct AcceptClientThreadParams {
    SOCKET serverSocket; /**< Server socket for accepting client connections */
    Queue* queue; /**< Pointer to the queue for task management */
    HANDLE* threadPoolClients; /**< Array of handles to worker threads */
    bool* threadPoolClientsStatus; /**< Array indicating the status of worker threads */
} AcceptClientThreadParams;

#endif // ACCEPT_CLIENT_THREAD_PARAMS_HPP
