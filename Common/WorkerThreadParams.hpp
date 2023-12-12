#ifndef WORKER_THREAD_PARAMS_HPP
#define WORKER_THREAD_PARAMS_HPP

#include <winsock2.h>
#include "MeasurementData.hpp"

typedef struct WorkerThreadParams {
	MeasurementData data;
	unsigned int threadId;
	bool* threadPoolWorkerStatus;
	CRITICAL_SECTION cs;
} WorkerThreadParams;

#endif // WORKER_THREAD_PARAMS_HPP
