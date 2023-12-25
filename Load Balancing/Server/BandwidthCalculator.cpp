#include "BandwidthCalculator.hpp"

#pragma region STATISTICS SERVICE HANDLER
unsigned int __stdcall BandwidthStatsHandlerProc(LPVOID lpParameter) {
    __try {
        void* params = (void*)lpParameter;
        if (params == NULL) {
            return 1; // Return an error code if params is NULL
        }
        else {
            BandwidthStatistics(params);
            return 0;
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return 1; // Return an error code indicating an exception occurred
    }
}
#pragma endregion

#pragma region INTELIGENT STATISTICS SERVICE
// Function to calculate data processed in KB/s in the last RTU_VALUE_SECONDS seconds
unsigned int __stdcall BandwidthStatistics(void* param) {
    const int interval = RTU_VALUE_SECONDS; // Time interval in seconds
    int lastDataProcessed = 0; // Last count of data processed in bytes
    time_t startTime = time(NULL);
    BandwidthThreadParams* params = (BandwidthThreadParams*)param;

    while (!ShutDown(params->queue)) {
        // Gracefully shutdown load balancer
        if (_kbhit() || (params->test_mode && QueueSize(params->queue) <= 0)) { // Check if a key has been pressed
            char ch = _getch(); // Get the pressed key
            if (ch == 'q' || ch == 'Q') {
                SetShutDown(params->queue);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down Load Balancer service...");
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down TCP handler service...");
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down Bandwidth Statistics service...\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                ExitThread(0);
            }
        }

        time_t currentTime = time(NULL);
        double elapsedSeconds = difftime(currentTime, startTime);

        if (elapsedSeconds >= interval) {
            // Perform QueueSize calculation and accumulate data processed
            int currentTotal = QueueTotalDataEnqueued(params->queue);

            // Difference between last cycle and new cycle
            int dataProcessed = currentTotal - lastDataProcessed;

            // Save last time data count
            lastDataProcessed = currentTotal;

            // Calculate data processed in KB/s
            double dataProcessedB = (double)dataProcessed * sizeof(MeasurementData);
            double dataProcessedPerSec = dataProcessedB / (double)interval;

            // Calculate worker utilization
            EnterCriticalSection(&params->cs);
            int workers_num = params->max_workers;
            int workers_busy = 0;
            for (int i = 0; i < workers_num; i++) {
                if (params->threadPoolWorkerStatus[i] == THREAD_BUSY) {
                    workers_busy++;
                }
            }
            LeaveCriticalSection(&params->cs);

            // Calculate bandwidth
            int bandwidth = (int)(((double)workers_busy / workers_num) * 100);

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
      
            if(params->test_mode == false) 
                printf("\n[Bandwidth Statistics]: Data processed in the last %d seconds: %.2f B/s\n", interval, dataProcessedPerSec * (workers_num * 0.35));
            
            if(params->test_mode)
                printf("\n[Bandwidth Statistics]: Currently in queue: %d samples\n", QueueSize(params->queue));

            printf("[Bandwidth Statistics]: Bandwidth in the last %d seconds: %2d%%\n\n", interval, bandwidth);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            // Reset values for the next interval
            startTime = time(NULL);
        }

        Sleep(1000); // Sleep for a short duration before checking again
    }

    return 0;
}
#pragma endregion