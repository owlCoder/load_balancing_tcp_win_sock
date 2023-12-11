#include "Worker.hpp"
#include "../Common/WorkerThreadParams.hpp"

unsigned int Process_Data(void *params) 
{
    WorkerThreadParams *workerParams = (WorkerThreadParams*)params;
    MeasurementData data = workerParams->data;
    unsigned int threadId = workerParams->threadId;

    srand((unsigned int)time(NULL));
    printf("Processing Data...\n");

    int start_value = data.measurementValue;
    data.measurementValue = rand() % 100; // Modify the measurement value (random value here)

    // Simulate job duration (sleep between 1 to 3.5 seconds)
    unsigned int sleep_time = (rand() % 2500) + 1000; // Random sleep time between 1 to 3.5 seconds
    Sleep(sleep_time * 1000); // sleep takes time in microseconds, hence *1000 for milliseconds

    // Returning thread ID if data was processed
    if (start_value != data.measurementValue)
        return threadId;
    else
        return -1;
}
