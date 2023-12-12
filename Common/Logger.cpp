#include "Logger.hpp"

// Helper function to log messages to a file
void LogToFile(const char* filename, const char* message) {
    FILE* file = fopen(filename, "a");
    if (file == nullptr) {
        fprintf(stderr, "Error opening file %s for logging\n", filename);
        return;
    }

    fprintf(file, "%s\n", message);
    fclose(file);
}