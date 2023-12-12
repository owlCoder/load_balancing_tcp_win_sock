#include "Configuration.hpp"

// Function to initialize the server socket
bool InitializeServerSocket(SOCKET* serverSocket) {
    *serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (*serverSocket == INVALID_SOCKET) {
        fprintf(stderr, "Error creating socket: %d\n", WSAGetLastError());
        return false;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(*serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind failed with error: %d\n", WSAGetLastError());
        closesocket(*serverSocket);
        return false;
    }

    if (listen(*serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(stderr, "Listen failed with error: %d\n", WSAGetLastError());
        closesocket(*serverSocket);
        return false;
    }

    printf("[Server]: Listening on port %d\n", PORT);
    return true;
}

// Function to initialize the server and thread pool
bool InitializeServer(Queue* queue, HANDLE* threadPoolClients, bool* threadPoolClientsStatus, HANDLE* threadPoolWorkers, bool* threadPoolWorkersStatus, SOCKET* serverSocket) {
    InitializeQueue(queue);

    for (int i = 0; i < MAX_CLIENTS_THREADS; ++i) {
        threadPoolClientsStatus[i] = THREAD_FREE;
    }

    for (int i = 0; i < MAX_WORKERS_THREADS; ++i) {
        threadPoolWorkersStatus[i] = THREAD_FREE;
    }

    return InitializeServerSocket(serverSocket);
}

// Function to start the server
void StartServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return;
    }

    Queue queue;
    HANDLE threadPoolClients[MAX_CLIENTS_THREADS];
    bool threadPoolClientsStatus[MAX_CLIENTS_THREADS];
    SOCKET serverSocket;

    // THREAD POOL FOR WORKERS
    HANDLE threadPoolWorkers[MAX_WORKERS_THREADS];
    bool threadPoolWorkersStatus[MAX_WORKERS_THREADS];

    // Multi client connections
    if (!InitializeServer(&queue, threadPoolClients, threadPoolClientsStatus, threadPoolWorkers, threadPoolWorkersStatus, &serverSocket)) {
        fprintf(stderr, "Server initialization failed\n");
        WSACleanup();
        return;
    }

    // Wait and accept multiple connections
    //AcceptClientConnections(serverSocket, &queue, threadPoolClients, threadPoolClientsStatus);

    // Run load balancing
    //RunLoadBalancer(&queue, threadPoolWorkers, threadPoolWorkersStatus);

    // Create structs with the necessary data for threads
    AcceptClientThreadParams acceptParams = { serverSocket, &queue, threadPoolClients, threadPoolClientsStatus };
    RunLoadBalancerThreadParams runParams = { &queue, threadPoolWorkers, threadPoolWorkersStatus };

    // Create threads using beginthreadex
    HANDLE acceptClientsThread = (HANDLE)_beginthreadex(NULL, 0, AcceptClientConnections, (void*)&acceptParams, 0, NULL);
    HANDLE runLoadBalancerThread = (HANDLE)_beginthreadex(NULL, 0, RunLoadBalancer, (void*)&runParams, 0, NULL);

    if (acceptClientsThread == 0 || runLoadBalancerThread == 0) {
        // Error handling: At least one thread creation failed
        if (acceptClientsThread != 0) {
            CloseHandle((HANDLE)acceptClientsThread);
        }
        if (runLoadBalancerThread != 0) {
            CloseHandle((HANDLE)runLoadBalancerThread);
        }
        // Additional error handling or return based on your application's logic
    }
    else {
        // Wait for threads to finish
        WaitForSingleObject((HANDLE)runLoadBalancerThread, INFINITE);
        WaitForSingleObject((HANDLE)acceptClientsThread, INFINITE);

        // Clean up resources
        CloseHandle((HANDLE)acceptClientsThread);
        CloseHandle((HANDLE)runLoadBalancerThread);

    }

    // Clean up resources
    for (int i = 0; i < MAX_CLIENTS_THREADS; ++i) {
        CloseHandle(threadPoolClients[i]);
    }

    // Clean up resources for workers
    for (int i = 0; i < MAX_WORKERS_THREADS; ++i) {
        CloseHandle(threadPoolWorkers[i]);
    }

    closesocket(serverSocket);
    WSACleanup();
}