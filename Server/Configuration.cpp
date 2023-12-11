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
bool InitializeServer(Queue* queue, HANDLE* threadPoolClients, bool* threadPoolClientsStatus, SOCKET* serverSocket) {
    InitializeQueue(queue);

    for (int i = 0; i < MAX_CLIENTS_THREADS; ++i) {
        threadPoolClientsStatus[i] = THREAD_FREE;
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

    if (!InitializeServer(&queue, threadPoolClients, threadPoolClientsStatus, &serverSocket)) {
        fprintf(stderr, "Server initialization failed\n");
        WSACleanup();
        return;
    }

    AcceptClientConnections(serverSocket, &queue, threadPoolClients, threadPoolClientsStatus);

    // Clean up resources
    for (int i = 0; i < MAX_CLIENTS_THREADS; ++i) {
        CloseHandle(threadPoolClients[i]);
    }

    closesocket(serverSocket);
    WSACleanup();
}