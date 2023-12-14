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



#endif // STRESS_TESTS_HPP
