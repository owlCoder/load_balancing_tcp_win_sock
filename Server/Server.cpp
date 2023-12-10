#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h> // For _beginthreadex
#include "../Common/MeasurementData.hpp"

#pragma comment(lib, "ws2_32.lib")

#define PORT 5059
#define MAX_CLIENTS 10

void HandleClient(void* clientSocketPtr) {
    SOCKET clientSocket = *((SOCKET*)clientSocketPtr);
    free(clientSocketPtr);

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
                    // TO DO: ADD TO QUEUE
                    ////////////////////////////////////
                    printf("[Server]: New measurement added into queue\n");

                    send(clientSocket, "Measurement added to queue\n", 27, 0);
                }
                else {
                    send(clientSocket, "Error parsing the received measurement\n", 40, 0);
                }
            }
            else {
                send(clientSocket, "Error receiving the packet from network\n", 41, 0);
            }
            free(new_data);
        }
    } while (bytesReceived > 0);

    printf("[Server]: Client disconnected\n");
    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        fprintf(stderr, "Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind failed with error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, MAX_CLIENTS) == SOCKET_ERROR) {
        fprintf(stderr, "Listen failed with error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("[Server]: Listening on port %d\n", PORT);

    while (1) {
        SOCKET clientSocket;
        struct sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);

        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize)) == INVALID_SOCKET) {
            fprintf(stderr, "Accept failed with error: %d\n", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        printf("[Server]: Client connected\n");

        // Create a thread to handle the client
        SOCKET* clientSocketPtr = (SOCKET*)malloc(sizeof(SOCKET));

        if (clientSocketPtr == NULL) {
            printf("[Server]: Socket wasn't initialized!\n");
            break;
        }

        *clientSocketPtr = clientSocket;
        _beginthreadex(NULL, 0, (_beginthreadex_proc_type)HandleClient, clientSocketPtr, 0, NULL);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}