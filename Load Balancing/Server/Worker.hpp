#ifndef WORKER_HPP
#define WORKER_HPP

#include "../Common/WorkerThreadParams.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <winsock2.h>
#include "time.h"

/**
 * @file Worker.hpp
 * @brief Header file containing the declaration of worker-related functionality.
 */

 /**
  * @brief Function to process measurement data.
  * @param data Measurement data to be processed.
  * @return unsigned int Returns an unsigned integer representing the process status.
  */
unsigned int Process_Data(MeasurementData data);

#endif // WORKER_HPP
