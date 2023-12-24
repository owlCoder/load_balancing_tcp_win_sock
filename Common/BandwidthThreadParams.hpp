#ifndef BANDWIDTH_THREAD_PARAMS_HPP
#define BANDWIDTH_THREAD_PARAMS_HPP

#include "Queue.hpp"

#define THREAD_BUSY true
#define THREAD_FREE false

typedef struct BandwidthThreadParams {
    Queue* queue;                    // Pointer to the queue
    bool* threadPoolWorkerStatus;    // Pointer to an array indicating worker status
    int max_workers;                 // Maximum number of workers
    bool test_mode;                  // Running in test mode
    CRITICAL_SECTION cs;             // Critical section
} BandwidthThreadParams;

#endif // BANDWIDTH_THREAD_PARAMS_HPP
