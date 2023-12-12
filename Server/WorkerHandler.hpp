#ifndef WORKER_HANDLER_HPP
#define WORKER_HANDLER_HPP

/**
 * @file WorkerHandler.hpp
 * @brief Header file containing the declaration of the worker function and necessary includes.
 */

#include <stdio.h>
#include <stdlib.h>
#include "Worker.hpp" // Include Worker.hpp for the Worker class
#include "../Common/WorkerThreadParams.hpp" // Include necessary headers for worker thread parameters

 /**
  * @brief Worker function definition.
  *
  * This function is responsible for handling the worker logic.
  * It takes a void pointer to arguments as input and returns an unsigned integer.
  *
  * @param args A pointer to the arguments passed to the worker function.
  * @return An unsigned integer representing the status or result of the worker operation.
  */
unsigned int Worker(void* args);

#endif // WORKER_HANDLER_HPP
