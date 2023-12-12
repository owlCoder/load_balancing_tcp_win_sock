#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdio>

/**
 * @brief Helper function to log a message to a file
 *
 * @param filename The name of the file to log to
 * @param message The message to be logged
 */
void LogToFile(const char* filename, const char* message);

#endif // LOGGER_HPP
