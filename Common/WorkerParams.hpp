#ifndef WORKER_PARAMS_HPP
#define WORKER_PARAMS_HPP

#include "Queue.hpp"

typedef struct WorkerParams
{
	Queue* queue;
	bool* workers_status;
	int max_workers;
	CRITICAL_SECTION cs;
} WorkerParams;

#endif
