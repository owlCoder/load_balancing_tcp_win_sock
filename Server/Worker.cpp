#include "Configuration.hpp"
#include "../Common/CriticalSectionWrapper.hpp"

unsigned int Process_Data(void *params) 
{
    WorkerThreadParams *workerParams = (WorkerThreadParams*)params;
    MeasurementData data = workerParams->data;
    unsigned int threadId = workerParams->threadId;

    char logMessage[100];
    snprintf(logMessage, sizeof(logMessage), "[Worker %u]: Processing Measurement Data...", threadId);

    // Log the message to the file
    LogToFile("../Logs/worker_log.txt", logMessage);

    int start_value = data.measurementValue;
    data.measurementValue = 0;

    // Simulate job duration (sleep between 2 to 7 seconds)
    srand((unsigned int)time(NULL));
    unsigned int sleep_time = (rand() % 5) + 2; // Random sleep time between 1 to 3 seconds
    Sleep(5000); // sleep takes time in microseconds, hence *1000 for milliseconds

    // Initialize the critical section
    InitializeCriticalSectionWrapper(&(workerParams->cs));

    // Enter critical section before changing
    EnterCriticalSectionWrapper(&(workerParams->cs));
    workerParams->threadPoolWorkerStatus[threadId] = THREAD_FREE;

    // Exit critical section after changing
    EnterCriticalSectionWrapper(&(workerParams->cs));

    // Returning thread ID if data was processed
    if (start_value != data.measurementValue)
        return threadId;
    else
        return -1;
}
