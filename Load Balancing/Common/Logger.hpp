#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdio>

/**
 * @brief Helper function to clear the contents of a log file
 *
 * @param filename The name of the file to clear
 */
inline void ClearLogFile(const char* filename) {
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, filename, "w");
    if (err != 0 || file == nullptr) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
        fprintf(stderr, "Error opening file %s for clearing\n", filename);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    fclose(file);
}

/**
 * @brief Helper function to log a message to a file
 *
 * @param filename The name of the file to log to
 * @param message The message to be logged
 */
inline void LogToFile(const char* filename, const char* message) {
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, filename, "a");
    if (err != 0 || file == nullptr) {
        return;
    }

    fprintf(file, "%s\n", message);
    fclose(file);
}

#endif // LOGGER_HPP
