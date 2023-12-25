#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdbool.h>
#include "../Common/ThreadParams.hpp"
#include "../Common/AcceptClientThreadParams.hpp"
#include "../Common/RunLoadBalancerThreadParams.hpp"
#include "../Common/WorkerParams.hpp"
#include "ClientHandler.hpp"
#include "Worker.hpp"
#include "LoadBalancer.hpp"
#include "BandwidthCalculator.hpp"
#include "Menu.hpp"
#include "conio.h"
#include "Colors.hpp"

#define PORT 5059
#define MAX_CLIENTS_THREADS 16
#define MAX_WORKERS_THREADS 5
#define THREAD_BUSY true
#define THREAD_FREE false

/**
 * @brief Initializes the server socket.
 *
 * @param[out] serverSocket The server socket to be initialized.
 * @return bool True if the server socket is successfully initialized, otherwise false.
 */
bool InitializeServerSocket(SOCKET* serverSocket);

/**
 * @brief Initializes the server including the queue and thread pool.
 *
 * @param[in,out] queue The queue for managing client connections.
 * @param[in,out] threadPoolClients The array of threads managing client connections.
 * @param[in,out] threadPoolClientsStatus The array keeping track of thread statuses.
 * @param[out] serverSocket The initialized server socket.
 * @return bool True if the server initialization is successful, otherwise false.
 */
bool InitializeServer(Queue* queue, HANDLE* threadPoolClients, bool* threadPoolClientsStatus, HANDLE* threadPoolWorkers, bool* threadPoolWorkersStatus, SOCKET* serverSocket);

/**
 * @brief Starts the server, handles client connections, and manages the thread pool.
 */
void StartServer();

#endif // CONFIGURATION_HPP
