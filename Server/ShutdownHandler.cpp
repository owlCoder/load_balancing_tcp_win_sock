#include "ShutdownHandler.hpp"

bool shutdownRequested = false;


// Function to change the value of shutdownRequested
void setShutdownFlag(bool value) {
    shutdownRequested = value;
}