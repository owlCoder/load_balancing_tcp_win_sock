#ifndef WORKER_THREAD_PARAMS_HPP
#define WORKER_THREAD_PARAMS_HPP

#include <winsock2.h>
#include "MeasurementData.hpp"

/**
 * @struct WorkerThreadParams
 * @brief Structure containing parameters for a worker thread.
 */
typedef struct WorkerThreadParams {
    MeasurementData data; /**< Measurement data for processing */
    unsigned int threadId; /**< Thread identifier */
    bool* threadPoolWorkerStatus; /**< Pointer to array indicating worker thread status */
    CRITICAL_SECTION cs; /**< Critical section for synchronization */
} WorkerThreadParams;

#endif // WORKER_THREAD_PARAMS_HPP
