#include "LoadBalancer.hpp"

unsigned int __stdcall RunLOadBalancer(void* param) {
    while (1)
    {
        // Gracefully shutdown load balancer
        if (_kbhit()) { // Check if a key has been pressed
            char ch = _getch(); // Get the pressed key
            if (ch == 'q' || ch == 'Q') {
                SetShutDown(((RunLoadBalancerThreadParams*)param)->queue);

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down Load Balancer service...");
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down TCP handler service...");
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down Bandwidth Statistics service...\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                ExitThread(0);
                return 0;
            }
        }

        RunLoadBalancerThreadParams* params = (RunLoadBalancerThreadParams*)param;

        Queue* queue = params->queue;
        HANDLE* threadPoolWorkers = params->threadPoolWorkers;
        bool* threadPoolWorkersStatus = params->threadPoolWorkersStatus;

        // Check is test mode, if yes when Queue reach size of 0 elements, exit
        if (params->test_mode && QueueSize(queue) <= 0) {
            SetShutDown(((RunLoadBalancerThreadParams*)param)->queue);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("\n[Intelligent Resource Manager]: Gracefully shutting down Load Balancer service...");
            printf("\n[Intelligent Resource Manager]: Gracefully shutting down Bandwidth Statistics service...\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return 0;
        }

        WorkerThreadParams* workerParams = (WorkerThreadParams*)malloc(sizeof(WorkerThreadParams));
        if (workerParams == NULL) {
            fprintf(stderr, "Memory allocation for workers failed\n");
            return 1; // Return error code or terminate the thread accordingly
        }

        int threadIndex = -1;
        for (int i = 0; i < MAX_WORKERS_THREADS; ++i) {
            if (threadPoolWorkersStatus[i] == THREAD_FREE) {
                threadIndex = i;
                break;
            }
        }

        if (threadIndex != -1) {
            // Dequeue data if available to pass to worker
            MeasurementData* data = Dequeue(queue);

            if (data != nullptr) {
                workerParams->data = *data;
                workerParams->threadId = -1; // no thread for worker has been allocated yet
                workerParams->threadPoolWorkerStatus = threadPoolWorkersStatus;
                workerParams->threadId = threadIndex;
                threadPoolWorkersStatus[threadIndex] = THREAD_BUSY; // Get thread from thread pool

                char logMessage[100];
                snprintf(logMessage, sizeof(logMessage), "[Worker Init]: Created handler with thread id % d for worker to process data.", threadIndex);
                LogToFile("../Logs/worker_log.txt", logMessage);

                threadPoolWorkers[threadIndex] = (HANDLE)_beginthreadex(NULL, 0, NULL, (void*)workerParams, 0, NULL);
            }
        }
        else {
            /* char logMessage[100];
             snprintf(logMessage, sizeof(logMessage), "[Worker Init]: All threads are busy. Maximum capacity used.");
             LogToFile("../Logs/worker_log.txt", logMessage);*/
            free(workerParams);
        }
    }

    // Print shutdown message
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("\n[Intelligent Resource Manager]: Gracefully shutting down Load Balancer service");
    Sleep(1000); printf(".");
    Sleep(1000); printf(".");
    Sleep(1000); printf(".");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    return 0;
}

#pragma region LOAD BALANCER SERVICE
unsigned int __stdcall RunLoadBalancer(void* param) {
    Queue* queue = ((RunLoadBalancerThreadParams*)param)->queue;

    while (1)
    {
        // Gracefully shutdown load balancer
        if (_kbhit() || (((RunLoadBalancerThreadParams*)param)->test_mode && QueueSize(queue) <= 0)) { // Check if a key has been pressed
            char ch = _getch(); // Get the pressed key
            if (ch == 'q' || ch == 'Q') {
                SetShutDown(((RunLoadBalancerThreadParams*)param)->queue);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down Load Balancer service...");
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down TCP handler service...");
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down Bandwidth Statistics service...\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);   
                ExitThread(0);
            }
        }
    }

    return 0;
}
#pragma endregion