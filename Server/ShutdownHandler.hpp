/**
 * @file ShutdownHandler.hpp
 * @brief Declaration of global variable for shutdown flag and related function.
 */

#ifndef SHUTDOWN_HANDLER_HPP
#define SHUTDOWN_HANDLER_HPP

#include <stdbool.h>

 /**
  * @brief Flag indicating the shutdown request status.
  *
  * This global variable is used to signal a shutdown request.
  * When set to true, it indicates that a shutdown has been requested.
  * This flag is intended to be accessed and modified across multiple files or functions.
  */
extern bool shutdownRequested;

/**
 * @brief Sets the value of the shutdown flag.
 *
 * This function modifies the value of the shutdownRequested flag.
 *
 * @param value The new value to be assigned to the shutdown flag (true or false).
 */
void setShutdownFlag(bool value);

#endif  // SHUTDOWN_HANDLER_HPP
