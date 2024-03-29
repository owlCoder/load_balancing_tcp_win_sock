#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Common/MeasurementData.hpp"
#include "../Common/RNGState.hpp"
#include "GenerateMeasurementDate.hpp"
#include "GenerateMeasurementValue.hpp"
#include "../Server/Colors.hpp" 

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define PORT 5059
#define SERVER_ADDRESS "127.0.0.1"

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return 1;
    }

    // Get console handle for color output
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        fprintf(stderr, "Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        fprintf(stderr, "Connection failed with error: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    SetConsoleTextAttribute(consoleHandle, FOREGROUND_LIGHTGREEN);
    printf("[Client]: Connected to Load Balancer service\n");

    while (1) {
        // Generate new seed on every iteration
        srand((unsigned int)time(NULL));

        // Allocate memory for new_data
        struct MeasurementData* new_data = (struct MeasurementData*)malloc(sizeof(struct MeasurementData));

        if (new_data == NULL) {
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_LIGHTRED);
            printf("\nError allocating memory for buffer!\n");
            break;
        }

        // Generate a random measurement value between 1 and 100,000
        struct RNGState rng_state {};
        struct RNGState rng_state_sleep {};
        rng_state.s[0] = rand() % 1000;
        rng_state.s[1] = rand() % 23233;
        srand((unsigned int)time(NULL));
        rng_state_sleep.s[0] = rand() % 123;
        rng_state_sleep.s[1] = rand() % 321;

        int random_value = xorshift128plus(&rng_state) % 100000 + 1;
        new_data->measurementValue = (unsigned int)random_value;

        // Generate measurement date
        GenerateMeasurementDate(new_data->date);

        // Pack the measurement value into a message
        char message[1024];
        snprintf(message, sizeof(message), "%s %u", new_data->date, new_data->measurementValue);

        size_t messageLength = strlen(message);
        send(clientSocket, message, (int)messageLength, 0);

        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            // Connection closed or error occurred
            if (bytesReceived == 0) {
                // Connection closed gracefully
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_LIGHTRED);
                printf("[Connection Handler]: Connection closed by the server\n");
            }
            else {
                // Error in receiving data
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_LIGHTRED);
                printf("[Connection Handler]: Connection closed by the server\n");
            }

            // Clean-up and exit
            closesocket(clientSocket);
            WSACleanup();
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_WHITE);
            printf("Press any key to close client...");
            char tmp = getchar();
            return 2;
        }

        if (bytesReceived > 0 && bytesReceived < 1023) {
            buffer[bytesReceived] = '\0';

            if (strcmp("Connection accepted\n", buffer) == 0) {
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_LIGHTGREEN);
                printf("[Load Balancer]: %s", buffer);
            }
            else if (strcmp("Measurement added to queue\n", buffer) == 0) {
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_LIGHTCYAN);
                printf("[Load Balancer]: %s", buffer);
            }
            else {
                // close connection because no free threads on server available
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_LIGHTRED);
                printf("[Connection Handler]: %s", buffer);
                closesocket(clientSocket);
                WSACleanup();
                printf("Press any key to close client...");
                char tmp = getchar();
                return 2;
            }
        }

        // Free the allocated memory
        free(new_data);

        // Sleep for a random period between 1000 to 3000 milliseconds
        Sleep(xorshift128plus(&rng_state_sleep) % 2000 + 1000);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
