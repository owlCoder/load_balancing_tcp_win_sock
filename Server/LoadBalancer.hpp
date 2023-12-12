#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

/**
 * @file Load_Balancer.hpp
 * @brief Header file containing declarations for load balancer functions and handlers.
 */

#include <WinSock2.h>
#include "../Common/Queue.hpp" // Include the header file for the Queue
#include "WorkerHandler.hpp" // Include the header file for the WorkerHandler

 /**
  * @brief Load balancer handler function definition.
  *
  * This function is responsible for handling the load balancing logic.
  * It takes a pointer to a parameter as input and returns a DWORD.
  *
  * @param lpParameter A pointer to the parameter passed to the load balancer handler function.
  * @return A DWORD indicating the status or result of the load balancing operation.
  */
unsigned int __stdcall LoadBalancerHandlerProc(LPVOID lpParameter);

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
