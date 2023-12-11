#define _CRT_SECURE_NO_WARNINGS

#include "ClientHandler.hpp"
#include "Configuration.hpp"
#include "../Common/CriticalSectionWrapper.hpp"

DWORD WINAPI ClientHandlerProc(LPVOID lpParameter) {
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

    CriticalSectionWrapper wrapper;
    InitializeCriticalSectionWrapper(&wrapper); // Initialize the critical section

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
                    //EnterCriticalSectionWrapper(&wrapper); // Enter critical section before Enqueue
                    Enqueue(queue, new_data); // Add received data to the thread-safe queue
                    //LeaveCriticalSectionWrapper(&wrapper); // Leave critical section after Enqueue

                    printf("[Server]: New measurement added into queue\n");

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

    printf("[Server]: Client disconnected\n");
    closesocket(clientSocket);

    // ovde vrv treba neki lock ili critical section zbog stetnog prep;itnja
    // todo
    threadPoolClientsStatus[threadId] = THREAD_FREE;
}

// Function to accept incoming client connections
void AcceptClientConnections(SOCKET serverSocket, Queue* queue, HANDLE* threadPoolClients, bool* threadPoolClientsStatus) {
    while (1) {
        SOCKET clientSocket;
        struct sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);

        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize)) == INVALID_SOCKET) {
            fprintf(stderr, "Accept failed with error: %d\n", WSAGetLastError());
            closesocket(serverSocket);
            return;
        }

        ThreadParams* params = (ThreadParams*)malloc(sizeof(ThreadParams));
        if (params == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            closesocket(clientSocket);
            closesocket(serverSocket);
            return;
        }

        params->clientSocket = clientSocket;
        params->queue = queue;
        params->threadId = -1; // no thread for client has been allocated
        params->threadPoolClientsStatus = threadPoolClientsStatus;

        int threadIndex = -1;
        for (int i = 0; i < MAX_THREADS; ++i) {
            if (threadPoolClientsStatus[i] == THREAD_FREE) {
                threadIndex = i;
                threadPoolClientsStatus[i] = THREAD_BUSY;
                break;
            }
        }

        if (threadIndex != -1) {
            params->threadId = threadIndex;
            send(clientSocket, "Connection accepted\n", 21, 0);
            printf("[Server]: Client connected on handler thread id %d\n", threadIndex);
            threadPoolClients[threadIndex] = CreateThread(NULL, 0, ClientHandlerProc, (LPVOID)params, 0, NULL);
        }
        else {
            send(clientSocket, "All threads are busy. Connection rejected.\n", 44, 0);
            fprintf(stderr, "[Connection Handler]: All threads are busy. Connection rejected.\n");
            closesocket(clientSocket);
            free(params);
        }
    }
}