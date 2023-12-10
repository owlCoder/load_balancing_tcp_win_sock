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

#endif // CLIENT_HANDLER_HPP
