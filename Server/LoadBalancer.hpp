#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

/**
 * @file Load_Balancer.hpp
 * @brief Header file containing declarations for load balancer functions and handlers.
 */

#include <WinSock2.h>
#include "../Common/Queue.hpp" // Include the header file for the Queue
#include "WorkerHandler.hpp" // Include the header file for the WorkerHandler
#include "WorkerHandler.hpp"
#include "Configuration.hpp"


/**
 * @brief Function to run the load balancer.
 *
 * This function initiates and manages the load balancer functionality.
 * It takes a pointer to a queue, a pointer to the worker thread pool, and a pointer to the status of worker threads.
 *
 * @param queue A pointer to the Queue used by the load balancer.
 * @param threadPoolWorkers A pointer to the worker thread pool.
 * @param threadPoolWorkersStatus A pointer to the status of worker threads.
 */
unsigned int __stdcall RunLoadBalancer(void *params);

#endif // LOAD_BALANCER_HPP
