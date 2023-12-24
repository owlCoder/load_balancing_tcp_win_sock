#ifndef WORKER_PARAMS_HPP
#define WORKER_PARAMS_HPP

#include "Queue.hpp"

/**
 * @struct WorkerParams
 * @brief Structure containing parameters for worker threads.
 */
typedef struct WorkerParams
{
    Queue* queue; /**< Pointer to the queue for task management */
    bool* workers_status; /**< Pointer to the array of worker statuses */
    int max_workers; /**< Maximum number of workers */
    CRITICAL_SECTION cs; /**< Critical section for synchronization */
} WorkerParams;

#endif
