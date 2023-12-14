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
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("[Interaction Service]: Press 'Q' or 'q' to shutdown server\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
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
    HANDLE threadPoolClients[MAX_CLIENTS_THREADS]{};
    bool threadPoolClientsStatus[MAX_CLIENTS_THREADS]{};
    SOCKET serverSocket;

    // THREAD POOL FOR WORKERS
    HANDLE threadPoolWorkers[MAX_WORKERS_THREADS]{};
    bool threadPoolWorkersStatus[MAX_WORKERS_THREADS]{};

    // Multi client connections
    if (!InitializeServer(&queue, threadPoolClients, threadPoolClientsStatus, threadPoolWorkers, threadPoolWorkersStatus, &serverSocket)) {
        fprintf(stderr, "Server initialization failed\n");
        WSACleanup();
        return;
    }

    // Create structs with the necessary data for threads
    AcceptClientThreadParams acceptParams = { serverSocket, &queue, threadPoolClients, threadPoolClientsStatus };
    RunLoadBalancerThreadParams runParams = { &queue, threadPoolWorkers, threadPoolWorkersStatus };

    // Create threads using beginthreadex
    HANDLE acceptClientsThread = (HANDLE)_beginthreadex(NULL, 0, AcceptClientConnections, (void*)&acceptParams, 0, NULL);
    HANDLE runLoadBalancerThread = (HANDLE)_beginthreadex(NULL, 0, RunLoadBalancer, (void*)&runParams, 0, NULL);

    HANDLE runBandwidthStatsThread = (HANDLE)_beginthreadex(NULL, 0, BandwidthStatsHandlerProc, (void*)&queue, 0, NULL);

    if (acceptClientsThread == NULL || runLoadBalancerThread == NULL || runBandwidthStatsThread == NULL) {
        // Error handling: At least one thread creation failed
        if (acceptClientsThread != NULL) {
            CloseHandle(acceptClientsThread);
        }
        if (runLoadBalancerThread != NULL) {
            CloseHandle(runLoadBalancerThread);
        }
        if (runBandwidthStatsThread != NULL) {
            CloseHandle(runBandwidthStatsThread);
        }
    }
    else {
        // Wait for threads to finish
        WaitForSingleObject((HANDLE)runLoadBalancerThread, INFINITE);
        WaitForSingleObject((HANDLE)runBandwidthStatsThread, INFINITE);

        // Clean up resources
        CloseHandle((HANDLE)runLoadBalancerThread);
        CloseHandle((HANDLE)acceptClientsThread);
        CloseHandle((HANDLE)runBandwidthStatsThread);

        // Clean up resources for client threads
             // Clean up resources for client threads
        for (int i = 0; i < MAX_CLIENTS_THREADS; ++i) {
            if (threadPoolClients[i] != NULL && threadPoolClients[i] != INVALID_HANDLE_VALUE) {
                CloseHandle(threadPoolClients[i]);
            }
        }

        // Clean up resources for worker threads
        for (int i = 0; i < MAX_WORKERS_THREADS; ++i) {
            if (threadPoolWorkers[i] != NULL && threadPoolWorkers[i] != INVALID_HANDLE_VALUE) {
                CloseHandle(threadPoolWorkers[i]);
            }
        }

    }

    closesocket(serverSocket);
    WSACleanup();
}