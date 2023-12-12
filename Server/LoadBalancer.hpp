#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include <WinSock2.h>
#include "../Common/Queue.hpp"

DWORD WINAPI LoadBalancerHandlerProc(LPVOID lpParameter);
void RunLoadBalancer(Queue *queue, HANDLE *threadPoolWorkers, bool *threadPoolWorkersStatus);

#endif // LOAD_BALANCER_HPP
