#ifndef RUN_LOAD_BALANCER_THREAD_PARAMS_HPP
#define RUN_LOAD_BALANCER_THREAD_PARAMS_HPP

#include "Queue.hpp"
#include <Windows.h>

// Struct to pass data to the RunLoadBalancer thread
typedef struct RunLoadBalancerThreadParams {
    Queue* queue;
    HANDLE* threadPoolWorkers;
    bool* threadPoolWorkersStatus;
} RunLoadBalancerThreadParams;

#endif // RUN_LOAD_BALANCER_THREAD_PARAMS_HPP
