#include "Configuration.hpp"
#include "../Common/CriticalSectionWrapper.hpp"

#pragma region WORKER PROCESSING DATA SERVICE
unsigned int Process_Data(MeasurementData data) 
{
    unsigned int threadId = (unsigned int)GetCurrentThreadId();
    char logMessage[100];

    snprintf(logMessage, sizeof(logMessage), "[Thread ID: %u]: Processing Measurement Data...", threadId);

    // Log the message to the file
    LogToFile("../Logs/worker_log.txt", logMessage);

    int start_value = data.measurementValue;
    data.measurementValue = 0;

    // Simulate job duration (sleep between 2 to 7 seconds)
    srand((unsigned int)start_value);
    unsigned int sleep_time = (rand() % 5) + 2; // Random sleep time between 2 to 7 seconds
    Sleep(sleep_time * 1000); // sleep takes time in microseconds, hence *1000 for milliseconds

    // Returning thread ID if data was processed
    if (start_value != data.measurementValue)
        return threadId;
    else
        return -1;
}
#pragma endregion
