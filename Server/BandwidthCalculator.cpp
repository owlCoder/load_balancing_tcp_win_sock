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

// Function to calculate data processed in KB/s in the last 60 seconds
unsigned int __stdcall BandwidthStatistics(void* param) {
    const int interval = 5; // Time interval in seconds
    int dataProcessed = 0; // Data processed in current cycle in bytes
    int lastDataProcessed = 0; // Last count of data processed in bytes
    time_t startTime = time(NULL);

    while (1) {
        time_t currentTime = time(NULL);
        double elapsedSeconds = difftime(currentTime, startTime);

        if (elapsedSeconds >= interval) {
            // Perform QueueSize calculation and accumulate data processed
            int currentTotal = QueueSize(((Queue*)param), 0);

            dataProcessed = currentTotal - lastDataProcessed;

            // Save last time data count
            lastDataProcessed = currentTotal;

            // Calculate data processed in KB/s
            double dataProcessedKB = (double)dataProcessed * sizeof(MeasurementData) / (1024);
            double dataProcessedPerSec = dataProcessedKB / (double)interval;

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("[Bandwidth Statistics]: Data processed in the last %d seconds: %.2f KB/s\n", interval, dataProcessedPerSec);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            // Reset values for the next interval
            startTime = time(NULL);
        }

        Sleep(1000); // Sleep for a short duration before checking again
    }

    return 0;
}