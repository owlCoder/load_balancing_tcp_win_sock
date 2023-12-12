#include "LoadBalancer.hpp"
#include "WorkerHandler.hpp"
#include "Configuration.hpp"

DWORD WINAPI LoadBalancerHandlerProc(LPVOID lpParameter)
{
    __try {
        WorkerThreadParams* params = (WorkerThreadParams*)lpParameter;
        if (params == NULL) {
            return 1; // Return an error code if params is NULL
        }
        else {
            Worker(params);
            return 0;
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        // Exception handling code here
        return 1; // Return an error code indicating an exception occurred
    }
}

void RunLoadBalancer(Queue* queue, HANDLE* threadPoolWorkers, bool* threadPoolWorkersStatus) {
    while (1) {
        // Dequeue data if available to pass to worker
        MeasurementData *data = Dequeue(queue);

        if (data != NULL) 
        {
            WorkerThreadParams* params = (WorkerThreadParams*)malloc(sizeof(WorkerThreadParams));
            if (params == NULL) {
                fprintf(stderr, "Memory allocation for workers failed\n");
                return;
            }

            params->data = *data;
            params->threadId = -1; // no thread for worker has been allocated yet
            params->threadPoolWorkerStatus = threadPoolWorkersStatus;

            int threadIndex = -1;
            for (int i = 0; i < MAX_WORKERS_THREADS; ++i) {
                if (threadPoolWorkersStatus[i] == THREAD_FREE) {
                    threadIndex = i;
                    threadPoolWorkersStatus[i] = THREAD_BUSY;
                    break;
                }
            }

            if (threadIndex != -1) {
                params->threadId = threadIndex;
                printf("[Worker Init]: Created handler with thread id %d for worker to process data\n", threadIndex);
                threadPoolWorkers[threadIndex] = CreateThread(NULL, 0, LoadBalancerHandlerProc, (LPVOID)params, 0, NULL);
            }
            else {
                fprintf(stderr, "[Worker Init]: All threads are busy. Maximum capacity used.\n");
                free(params);
            }
        }
    }
}