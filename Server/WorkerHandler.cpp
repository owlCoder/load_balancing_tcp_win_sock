#include "WorkerHandler.hpp"

DWORD WINAPI Worker(LPVOID lpParam) {
    WorkerParams *wp = (WorkerParams*)lpParam;
    Queue* queue = wp -> queue;
    bool* workers_status = wp->workers_status;
    char logMessage[100];

    while (!ShutDown(queue))
    {
        MeasurementData *data = Dequeue(queue);

        if (data == nullptr) continue;

        EnterCriticalSection(&(wp->cs));
        int index = -1;
        for (int i = 0; i < wp->max_workers; i++) {
            if (workers_status[i] == false) { // is thread free
                workers_status[i] = true; // THREAD IS BUSY
                index = i;
            }
        }
        LeaveCriticalSection(&(wp->cs));

        if (index == -1) continue; // no free threads

        unsigned int result = Process_Data(*data);
        workers_status[index] = false; // return thread to thread pool
        
        if (result != -1)
            snprintf(logMessage, sizeof(logMessage), "[Worker %u]: Job done. Returning thread to thread pool...", result);
        else
            snprintf(logMessage, sizeof(logMessage), "[Worker Error Handler]: Error processing data!");

        LogToFile("../Logs/worker_log.txt", logMessage);
    }
    
    return 0;
}