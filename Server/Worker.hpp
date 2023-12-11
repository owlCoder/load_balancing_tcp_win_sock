#ifndef WORKER_HPP
#define WORKER_HPP

#include "../Common/MeasurementData.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <winsock2.h>
#include "time.h"

/**
 * @file Worker.hpp
 * @brief Header file defining the function prototype for data processing by workers.
 *
 * This header file contains the declaration of the Process_Data function,
 * responsible for processing MeasurementData by worker threads.
 */

 /**
  * @brief Process MeasurementData by worker threads.
  *
  * This function is responsible for processing the received MeasurementData by worker threads.
  * It takes a pointer to a MeasurementData struct and performs necessary processing.
  * Implementations of this function can perform specific data processing tasks as required.
  *
  * @param data A pointer to the MeasurementData struct to be processed.
  * @param threadId An id of thread pool got by LoadBalancer
  * @return An integer indicating the thread ID upon successful completion of the job,
  *         or -1 if an error occurs during data processing.
  */
unsigned int Process_Data(MeasurementData* data, unsigned int threadId);

#endif // WORKER_HPP
