#define _CRT_SECURE_NO_WARNINGS

#include "ClientHandler.hpp"
#include "Configuration.hpp"
#include "../Common/CriticalSectionWrapper.hpp"

unsigned int __stdcall ClientHandlerProc(LPVOID lpParameter) {
    __try {
        ThreadParams* params = (ThreadParams*)lpParameter;
        if (params == NULL) {
            return 1; // Return an error code if params is NULL
        }
        else {
            HandleClient(params);
            return 0;
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        // Exception handling code here
        return 1; // Return an error code indicating an exception occurred
    }
}

void HandleClient(void* params) {
    ThreadParams* threadParams = (ThreadParams*)params;

    SOCKET clientSocket = threadParams->clientSocket;
    Queue* queue = threadParams->queue;
    bool* threadPoolClientsStatus = threadParams->threadPoolClientsStatus;
    int threadId = threadParams->threadId;

    char buffer[1024];
    int bytesReceived = -1;

    do {
        // Allocate memory for new_data
        struct MeasurementData* new_data = (struct MeasurementData*)malloc(sizeof(struct MeasurementData));

        if (new_data == NULL) {
            send(clientSocket, "Measurement can't be processed right now\n", 42, 0);
        }
        else {
            bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived > 0 && bytesReceived < 1023) {
                buffer[bytesReceived] = '\0';

                if (sscanf(buffer, "%s %u", new_data->date, &(new_data->measurementValue)) == 2) {
                    Enqueue(queue, new_data); // Add received data to the thread-safe queue
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    printf("[Data Processer]: Client %d measurement data has been added into queue\n", threadId);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    send(clientSocket, "Measurement added to queue\n", 27, 0);
                }
                else {
                    send(clientSocket, "Error parsing the received measurement\n", 40, 0);
                    free(new_data);
                }
            }
            else {
                send(clientSocket, "Error receiving the packet from network\n", 41, 0);
                free(new_data);
            }
        }
    } while (bytesReceived > 0);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("[Client Handler]: Client disconnected\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    closesocket(clientSocket);

    // Initialize the critical section
    InitializeCriticalSectionWrapper(&(threadParams->cs)); 

    // Enter critical section before changing
    EnterCriticalSectionWrapper(&(threadParams->cs)); 
    threadPoolClientsStatus[threadId] = THREAD_FREE;

    // Exit critical section after changing
    EnterCriticalSectionWrapper(&(threadParams->cs));
}

// Function to accept incoming client connections
unsigned int __stdcall AcceptClientConnections(void* param) {
    AcceptClientThreadParams* params = (AcceptClientThreadParams*)param;

    SOCKET serverSocket = params->serverSocket;
    Queue* queue = params->queue;
    HANDLE* threadPoolClients = params->threadPoolClients;
    bool* threadPoolClientsStatus = params->threadPoolClientsStatus;
    
    while (!shutdownRequested) {
        // Gracefully shutdown load balancer
        if (_kbhit()) { // Check if a key has been pressed
            char ch = _getch(); // Get the pressed key
            if (ch == 'q' || ch == 'Q') {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down TCP handler service...");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                setShutdownFlag(true);
                return 0;
            }
        }

        SOCKET clientSocket;
        struct sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);

        // Attempt to accept an incoming connection
        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize)) == INVALID_SOCKET) {
            int errorCode = WSAGetLastError();

            if (errorCode == WSAEWOULDBLOCK) {
                // No incoming connections pending, continue loop or perform other tasks
                continue;
            }
            else {
                // Handle other errors
                // Example: Print error message and break the loop or handle accordingly
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                fprintf(stderr, "[WinSock]: Accept failed with error: %d\n", errorCode);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                closesocket(serverSocket);
                return 1; // Return error code or terminate the thread accordingly
            }
        }

        ThreadParams* threadParams = (ThreadParams*)malloc(sizeof(ThreadParams));
        if (threadParams == NULL) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            fprintf(stderr, "[Client Handler Allocator]: Memory allocation failed\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            closesocket(clientSocket);
            closesocket(serverSocket);
            return 1; // Return error code or terminate the thread accordingly
        }

        threadParams->clientSocket = clientSocket;
        threadParams->queue = queue;
        threadParams->threadId = -1; // no thread for client has been allocated
        threadParams->threadPoolClientsStatus = threadPoolClientsStatus;

        int threadIndex = -1;
        for (int i = 0; i < MAX_CLIENTS_THREADS; ++i) {
            if (threadPoolClientsStatus[i] == THREAD_FREE) {
                threadIndex = i;
                threadPoolClientsStatus[i] = THREAD_BUSY;
                break;
            }
        }

        if (threadIndex != -1) {
            threadParams->threadId = threadIndex;
            send(clientSocket, "Connection accepted\n", 21, 0);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("\n[Server]: Client connected on handler thread id %d\n", threadIndex);
            threadPoolClients[threadIndex] = (HANDLE)_beginthreadex(NULL, 0, ClientHandlerProc, (void*)threadParams, 0, NULL);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        else {
            send(clientSocket, "All threads are busy. Connection rejected.\n", 44, 0);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            fprintf(stderr, "[Connection Handler]: All threads are busy. Connection rejected.\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            closesocket(clientSocket);
            free(threadParams);
        }

    }

    return 0;
}