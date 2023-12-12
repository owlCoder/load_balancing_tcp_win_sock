#include "Configuration.hpp"
#include "../Common/CriticalSectionWrapper.hpp"

unsigned int Process_Data(void *params) 
{
    WorkerThreadParams *workerParams = (WorkerThreadParams*)params;
    MeasurementData data = workerParams->data;
    unsigned int threadId = workerParams->threadId;

    srand((unsigned int)time(NULL));
    printf("[Worker %u]: Processing Measurement Data...\n", threadId);
    printf("\n\n\n%u\n", data.measurementValue);
    int start_value = data.measurementValue;
    data.measurementValue = 0;

    // Simulate job duration (sleep between 1 to 3.5 seconds)
    //unsigned int sleep_time = (rand() % 1000) + 1000; // Random sleep time between 1 to 2 seconds
    //Sleep(sleep_time * 1000); // sleep takes time in microseconds, hence *1000 for milliseconds
    printf("\n\n\n%u\n", data.measurementValue);
    Sleep(1000);
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
