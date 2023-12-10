#define _CRT_SECURE_NO_WARNINGS

#include "ClientHandler.hpp"

void HandleClient(void* params) {
    ThreadParams* threadParams = (ThreadParams*)params;

    SOCKET clientSocket = threadParams->clientSocket;
    Queue* queue = threadParams->queue;

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
    free(params); // Free allocated memory for thread parameters
}