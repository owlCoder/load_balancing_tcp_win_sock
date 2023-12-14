#include "BandwidthCalculator.hpp"

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

// Function to calculate data processed in KB/s in the last RTU_VALUE_SECONDS seconds
unsigned int __stdcall BandwidthStatistics(void* param) {
    const int interval = RTU_VALUE_SECONDS; // Time interval in seconds
    int lastDataProcessed = 0; // Last count of data processed in bytes
    time_t startTime = time(NULL);
    BandwidthThreadParams* params = (BandwidthThreadParams*)param;

    while (!ShutDown(params->queue)) {
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
            double dataProcessedKB = (double)dataProcessed * sizeof(MeasurementData) / (1024);
            double dataProcessedPerSec = dataProcessedKB / (double)interval;

            // Calculate worker utilization
            EnterCriticalSection(&params->cs);
            int workers_num = params->max_workers;
            int worker_busy = 0;
            for (int i = 0; i < workers_num; i++) {
                if (params->threadPoolWorkerStatus[i] == THREAD_BUSY) {
                    worker_busy++;
                }
            }
            LeaveCriticalSection(&params->cs);

            // Calculate bandwidth
            int bandwidth = (int)(((double)worker_busy / workers_num) * 100);

            // Calculate maximum possible data processed by workers
            int maxDataProcessed = (int)(((double)worker_busy / workers_num) * dataProcessed);

            // Calculate actual bandwidth based on data processed and maximum possible data processed
            int actualBandwidth = (int)(((double)dataProcessed / maxDataProcessed) * 100);

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("[Bandwidth Statistics]: Queue processed in the last %d seconds: %d\n", interval, QueueSize(params->queue));
            printf("[Bandwidth Statistics]: Data processed in the last %d seconds: %.2f KB/s\n", interval, dataProcessedPerSec);
            printf("[Bandwidth Statistics]: Bandwidth in the last %d seconds: %2d%%\n", interval, (actualBandwidth < 0 ? 0 : actualBandwidth));

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            // Reset values for the next interval
            startTime = time(NULL);
        }

        Sleep(1000); // Sleep for a short duration before checking again
    }

    return 0;
}