#ifndef RUN_LOAD_BALANCER_THREAD_PARAMS_HPP
#define RUN_LOAD_BALANCER_THREAD_PARAMS_HPP

#include "Queue.hpp"
#include <Windows.h>

/**
 * @struct RunLoadBalancerThreadParams
 * @brief Structure to pass data to the RunLoadBalancer thread.
 */
typedef struct RunLoadBalancerThreadParams {
    Queue* queue; /**< Pointer to the queue for task management */
    HANDLE* threadPoolWorkers; /**< Array of handles to worker threads */
    bool* threadPoolWorkersStatus; /**< Array indicating the status of worker threads */
    bool test_mode; /**< Flag indicating test mode */
} RunLoadBalancerThreadParams;

#endif // RUN_LOAD_BALANCER_THREAD_PARAMS_HPP
