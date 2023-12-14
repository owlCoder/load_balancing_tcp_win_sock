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

// Function to calculate data processed in MB/s in the last 60 seconds
unsigned int __stdcall BandwidthStatistics(void* param) {
    const int interval = 5; // Time interval in seconds
    int dataProcessed = 0; // Total data processed in bytes
    time_t startTime = time(NULL);

    while (1) {
        time_t currentTime = time(NULL);
        double elapsedSeconds = difftime(currentTime, startTime);

        if (elapsedSeconds >= interval) {
            // Calculate data processed in MB/s
            double dataProcessedMB = (double)dataProcessed / (1024);
            double dataProcessedPerSec = dataProcessedMB / interval;

            printf("[Bandwidth Statistics]: Data processed in the last %d seconds: %.2f KB/s\n", interval, dataProcessedPerSec);

            // Reset values for the next interval
            startTime = time(NULL);
            dataProcessed = 0;
        }

        // Perform QueueSize calculation and accumulate data processed
        int queueSize = QueueSize(((Queue *)param), dataProcessed); 
        dataProcessed += queueSize;

        Sleep(1000); // Sleep for a short duration before checking again
    }

    return 0;
}