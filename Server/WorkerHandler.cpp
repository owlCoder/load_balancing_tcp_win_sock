#include "WorkerHandler.hpp"
#include "../Common/Logger.hpp"

unsigned int Worker(void* args) {
    WorkerThreadParams* workerParams = (WorkerThreadParams*)args;
    unsigned int result = Process_Data(workerParams);

    // Handle the result (thread ID or error)
    char logMessage[100];

    if (result != -1) {
        snprintf(logMessage, sizeof(logMessage), "[Worker %u]: Job done. Returning thread to thread pool...", result);
    }
    else {
        snprintf(logMessage, sizeof(logMessage), "[Worker Error Handler]: Error processing data!");
    }

    LogToFile("../Logs/worker_log.txt", logMessage);

    free(workerParams); // Free allocated memory for  worker params
    return result;
}