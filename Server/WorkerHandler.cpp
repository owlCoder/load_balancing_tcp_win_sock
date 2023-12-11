#include "WorkerHandler.hpp"

unsigned int worker(void* args) {
    WorkerThreadParams* workerParams = (WorkerThreadParams*)args;
    unsigned int result = Process_Data(workerParams);

    // Handle the result (thread ID or error)
    if (result != -1) {
        printf("Job done by Thread ID: %u\n", result);
    }
    else {
        printf("Error processing data in worker thread\n");
    }

    free(workerParams); // Free allocated memory for parameters
    return result;
}