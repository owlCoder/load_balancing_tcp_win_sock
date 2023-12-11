#ifndef THREAD_PARAMS_HPP
#define THREAD_PARAMS_HPP

#include <winsock2.h>
#include "../Server/ThreadSafeQueue.hpp"

typedef struct ThreadParams {
    SOCKET clientSocket;
    Queue* queue;
    int threadId;
    bool* threadPoolClientsStatus;
    CRITICAL_SECTION cs; /**< The actual Critical Section object. */
} ThreadParams;

#endif // THREAD_PARAMS_HPP
