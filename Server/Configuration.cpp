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
    
    //  Show current configuration of server
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf(
        "[Configuration Service]: Applying configuration...\n"
        "\"server_config.json\": {\n"
        "\t\"MAX_TCP_CLIENTS_THREADS\": %u,\n"
        "\t\"MAX_WORKERS_THREADS\": %u,\n"
        "\t\"GC_LB_CRT\": %u,\n"
        "\t\"RTU_VALUE_SECONDS\": %u,\n"
        "\t\"THREADEX_S\": %u\n"
        "}\n"
        "[Configuration Service]: Configuration has been applied successfully\n",
        MAX_CLIENTS_THREADS, MAX_WORKERS_THREADS, 4, 60, 1);

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
    BandwidthThreadParams statsParams = { &queue, threadPoolWorkersStatus, MAX_WORKERS_THREADS };

    // Initialize critical section for stats params
    InitializeCriticalSection(&statsParams.cs);

    // Create threads using beginthreadex
    HANDLE acceptClientsThread = (HANDLE)_beginthreadex(NULL, 0, AcceptClientConnections, (void*)&acceptParams, 0, NULL);
    HANDLE runLoadBalancerThread = (HANDLE)_beginthreadex(NULL, 0, RunLoadBalancer, (void*)&runParams, 0, NULL);

    HANDLE runBandwidthStatsThread = (HANDLE)_beginthreadex(NULL, 0, BandwidthStatsHandlerProc, (void*)&statsParams, 0, NULL);

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

bool RunBandwidthTest(int workers_count) {
    Queue queue;

    InitializeQueue(&queue);

    // THREAD POOL FOR WORKERS (Dynamic Allocation)
    HANDLE* threadPoolWorkers = (HANDLE*)malloc(workers_count * sizeof(HANDLE));
    bool* threadPoolWorkersStatus = (bool*)malloc(workers_count * sizeof(bool));

    if (threadPoolWorkers == NULL || threadPoolWorkersStatus == NULL) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("\n[Thread Pool Allocator]: Error allocating memory for thread pool!");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return false;
    }

    // Initialize arrays with default values
    memset(threadPoolWorkers, 0, workers_count * sizeof(HANDLE));
    memset(threadPoolWorkersStatus, 0, workers_count * sizeof(bool));

    RunLoadBalancerThreadParams runParams = { &queue, threadPoolWorkers, threadPoolWorkersStatus };
    BandwidthThreadParams statsParams = { &queue, threadPoolWorkersStatus, workers_count };

    // Initialize critical section for stats params
    InitializeCriticalSection(&statsParams.cs);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf(
        "\n[Configuration Service]: Applying configuration...\n"
        "\"server_config.json\": {\n"
        "\t\"MAX_TCP_CLIENTS_THREADS\": %u,\n"
        "\t\"MAX_WORKERS_THREADS\": %u,\n"
        "\t\"GC_LB_CRT\": %u,\n"
        "\t\"RTU_VALUE_SECONDS\": %u,\n"
        "\t\"THREADEX_S\": %u\n"
        "}\n"
        "[Configuration Service]: Configuration has been applied successfully\n",
        MAX_CLIENTS_THREADS, MAX_WORKERS_THREADS, 4, 60, 1);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\n[Test Resource Generator]: Generating test measurement data");
    Sleep(1000); printf(".");
    Sleep(1000); printf(".");
    Sleep(1000); printf(".");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Add test data in queue on eg. number of workers * 10
    for (int i = 0; i < workers_count * 10; i++) {
        // Generate new seed on every iteration
        srand((unsigned int)time(NULL));

        // Allocate memory for new_data
        struct MeasurementData* new_data = (struct MeasurementData*)malloc(sizeof(struct MeasurementData));

        if (new_data == NULL) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("\n[Queue Allocator]: Error allocating memory for buffer. Exiting...");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return false;
        }

        new_data->measurementValue = (unsigned int)i + 1;

        // Generate measurement date
        int year = rand() % (2023 - 2000 + 1) + 2000;
        int month = rand() % 12 + 1;
        int day;

        switch (month) {
        case 2: // February
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                day = rand() % 29 + 1; // Leap year
            }
            else {
                day = rand() % 28 + 1; // Non-leap year
            }
            break;
        case 4: case 6: case 9: case 11: // April, June, September, November
            day = rand() % 30 + 1;
            break;
        default:
            day = rand() % 31 + 1;
        }

        sprintf_s(new_data->date, "%02d.%02d.%04d", day, month, year);

        // Add generate data into queue
        Enqueue(&queue, new_data);
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_LIGHTGREEN);
    printf("\n[Test Resource Generator]: Generating test data completed. %d measurement data has been added!", workers_count * 10);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_LIGHTYELLOW);
    printf("\n[Intelligent Background Service Runner]: Initializating threads");
    Sleep(1000); printf(".");
    Sleep(1000); printf(".");
    Sleep(1000); printf(".");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_YELLOW);
    printf("\n[Intelligent Background Service Runner]: Threads are up!");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\n[Interaction Service]: Press 'Q' or 'q' to cancel bandwidth test");

    
    // Create threads using beginthreadex
    HANDLE runLoadBalancerThread = (HANDLE)_beginthreadex(NULL, 0, RunLoadBalancer, (void*)&runParams, 0, NULL);

    HANDLE runBandwidthStatsThread = (HANDLE)_beginthreadex(NULL, 0, BandwidthStatsHandlerProc, (void*)&statsParams, 0, NULL);

    if (runLoadBalancerThread == NULL || runBandwidthStatsThread == NULL) {
        if (runLoadBalancerThread != NULL) {
            CloseHandle(runLoadBalancerThread);
        }
        if (runBandwidthStatsThread != NULL) {
            CloseHandle(runBandwidthStatsThread);
        }

        return false;
    }
    else {
        // Wait for threads to finish
        WaitForSingleObject((HANDLE)runLoadBalancerThread, INFINITE);
        WaitForSingleObject((HANDLE)runBandwidthStatsThread, INFINITE);

        // Clean up resources
        CloseHandle((HANDLE)runLoadBalancerThread);
        CloseHandle((HANDLE)runBandwidthStatsThread);

        // Clean up resources for worker threads
        for (int i = 0; i < workers_count; ++i) 
            CloseHandle(threadPoolWorkers[i]);
    }

    free(threadPoolWorkers);
    free(threadPoolWorkersStatus);

    return true;
}