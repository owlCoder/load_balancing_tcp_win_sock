#ifndef WORKER_THREAD_PARAMS_HPP
#define WORKER_THREAD_PARAMS_HPP

#include "MeasurementData.hpp"

typedef struct WorkerThreadParams {
	MeasurementData data;
	unsigned int threadId;
} WorkerThreadParams;

#endif // WORKER_THREAD_PARAMS_HPP
