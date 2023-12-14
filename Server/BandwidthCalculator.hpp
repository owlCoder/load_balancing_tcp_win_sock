#ifndef BANDWIDTH_CALCULATOR_HPP
#define BANDWIDTH_CALCULATOR_HPP

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdbool.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "../Common/Queue.hpp"
#include "ThreadSafeQueue.hpp"
#include "../Common/BandwidthThreadParams.hpp"

#define RTU_VALUE_SECONDS 15

/**
 * @file BandwidthCalculator.hpp
 * @brief Declaration of bandwidth calculation functions for thread handling.
 */

 /**
  * @brief Thread handler for calculating bandwidth statistics.
  *
  * This function serves as a handler for computing bandwidth statistics in a separate thread.
  *
  * @param lpParameter A pointer to the parameters passed to the thread handler.
  * @return An unsigned integer indicating the status of the thread handler execution.
  */
unsigned int __stdcall BandwidthStatsHandlerProc(LPVOID lpParameter);

/**
 * @brief Function to calculate bandwidth statistics.
 *
 * This function calculates the amount of data processed in MB/s in the last 60 seconds.
 * It measures the queue size at regular intervals to compute the processed data rate.
 *
 * @param param Pointer to the necessary parameters for calculating bandwidth statistics.
 * @return An unsigned integer indicating the status of the bandwidth statistics computation.
 */
unsigned int __stdcall BandwidthStatistics(void* param);

#endif // BANDWIDTH_CALCULATOR_HPP
