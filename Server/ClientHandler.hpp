#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <process.h> // For _beginthreadex
#include "../Common/ThreadParams.hpp"

#pragma comment(lib, "ws2_32.lib")

/**
 * @file ClientHandler.hpp
 * @brief Header file defining the function prototype for handling client connections.
 *
 * This header file contains the declaration of the HandleClient function,
 * responsible for handling client connections in a multi-threaded server application.
 */

 /**
  * @brief Handles client connections and data processing in a multi-threaded server.
  *
  * This function is responsible for handling individual client connections in a multi-threaded server.
  * It receives a pointer to a structure containing necessary parameters for processing client data.
  *
  * @param params A void pointer to the structure containing client connection parameters.
  */
void HandleClient(void* params);

/**
 * @brief Thread procedure to handle client connections.
 *
 * This function is responsible for handling client connections in a server application.
 * It accepts incoming client connections, creates a thread pool, and assigns client handling tasks
 * to available threads in the pool using a thread-safe queue.
 *
 * @param lpParameter A pointer to the thread parameters passed to the thread function.
 * @return Returns 0 on successful completion, or an error code indicating an exception occurred.
 */
DWORD WINAPI ClientHandlerProc(LPVOID lpParameter);


/**
 * @brief Accepts incoming client connections.
 *
 * @param[in] serverSocket The server socket to accept connections.
 * @param[in] queue The queue for managing client connections.
 * @param[in,out] threadPoolClients The array of threads managing client connections.
 * @param[in,out] threadPoolClientsStatus The array keeping track of thread statuses.
 */
void AcceptClientConnections(SOCKET serverSocket, Queue* queue, HANDLE* threadPoolClients, bool* threadPoolClientsStatus);

#endif // CLIENT_HANDLER_HPP
