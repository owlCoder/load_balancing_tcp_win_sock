#include "WorkerHandler.hpp"

unsigned int Worker(void* args) {
    WorkerThreadParams* workerParams = (WorkerThreadParams*)args;
    unsigned int result = Process_Data(workerParams);

    // Handle the result (thread ID or error)
    if (result != -1) {
        printf("[Worker %u]: Job done. Returning thread to thread pool...\n", result);
    }
    else {
        printf("[Worker Error Handler]: Error processing data!\n");
    }

    free(workerParams); // Free allocated memory for  worker params
    return result;
}