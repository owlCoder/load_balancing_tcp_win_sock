#ifndef BANDWIDTH_THREAD_PARAMS_HPP
#define BANDWIDTH_THREAD_PARAMS_HPP

#include "Queue.hpp"

#define THREAD_BUSY true
#define THREAD_FREE false

/**
 * @struct BandwidthThreadParams
 * @brief Structure containing parameters for bandwidth threads.
 */
typedef struct BandwidthThreadParams {
    Queue* queue; /**< Pointer to the queue for task management */
    bool* threadPoolWorkerStatus; /**< Pointer to an array indicating worker status */
    int max_workers; /**< Maximum number of workers */
    bool test_mode; /**< Flag indicating test mode */
    CRITICAL_SECTION cs; /**< Critical section for synchronization */
} BandwidthThreadParams;

#endif // BANDWIDTH_THREAD_PARAMS_HPP
