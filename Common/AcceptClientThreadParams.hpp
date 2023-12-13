#ifndef ACCEPT_CLIENT_THREAD_PARAMS_HPP
#define ACCEPT_CLIENT_THREAD_PARAMS_HPP

#include <WinSock2.h>
#include "Queue.hpp"

// Struct to pass data to the AcceptClientConnections thread
typedef struct AcceptClientThreadParams {
    SOCKET serverSocket;
    Queue* queue;
    HANDLE* threadPoolClients;
    bool* threadPoolClientsStatus;
} AcceptClientThreadParams;

#endif // ACCEPT_CLIENT_THREAD_PARAMS_HPP
