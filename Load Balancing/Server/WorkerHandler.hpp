#ifndef WORKER_HANDLER_HPP
#define WORKER_HANDLER_HPP

/**
 * @file WorkerHandler.hpp
 * @brief Header file containing the declaration of the worker function and necessary includes.
 */

#include <stdio.h>
#include <stdlib.h>
#include "Worker.hpp" /**< Include Worker.hpp for the Worker class */
#include "../Common/WorkerParams.hpp" /**< Include necessary headers for worker thread parameters */
#include "../Common/Logger.hpp"
#include "ThreadSafeQueue.hpp"

 /**
  * @brief Worker function to be executed by worker threads.
  * @param lpParam A pointer to the parameters for the worker thread.
  * @return DWORD Returns the thread exit code.
  */
DWORD WINAPI Worker(LPVOID lpParam);

#endif // WORKER_HANDLER_HPP
