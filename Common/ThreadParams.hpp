#ifndef THREAD_PARAMS_HPP
#define THREAD_PARAMS_HPP

#include <winsock2.h>
#include "../Server/ThreadSafeQueue.hpp"

typedef struct ThreadParams {
    SOCKET clientSocket;
    Queue* queue;
    int threadId;
    bool* threadPoolClientsStatus;
} ThreadParams;

#endif // THREAD_PARAMS_HPP
