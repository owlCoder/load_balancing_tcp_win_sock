#ifndef STRESS_TESTS_HPP
#define STRESS_TESTS_HPP

#include "ThreadSafeQueue.hpp"

/**
 * @brief Runs a stress test to evaluate the queue's memory capacity.
 *
 * This function simulates memory allocation and deallocation in a thread-safe queue,
 * measuring the allocated memory and freed memory after every specified number of allocations.
 *
 * @param max_allocations The maximum number of allocations to be performed in the test.
 * @return True if the test ran successfully, false otherwise.
 */
bool RunQueueCapacityTest(const int max_allocations, const int logFrequency);

/* Function implemented in Configuration.cpp */
/**
 * @brief Runs a bandwidth test with a specified number of workers.
 *
 * This function initiates a bandwidth test by creating a queue and a thread pool for workers.
 * It runs a load balancer thread and a thread to calculate bandwidth statistics based on the
 * provided number of workers.
 *
 * @param workers_count The number of workers for the bandwidth test.
 * @return Returns true if the bandwidth test runs successfully, otherwise returns false.
 *
 * @remarks The function creates a queue and a thread pool for workers to simulate processing
 * tasks. It then launches a load balancer thread and a thread to calculate bandwidth statistics
 * based on the provided number of workers. If any memory allocation fails or thread creation
 * encounters an issue, the function returns false. When the test completes or encounters an error,
 * it cleans up allocated resources and returns true or false accordingly.
 */
bool RunBandwidthTest(int workers_count);

#endif // STRESS_TESTS_HPP
