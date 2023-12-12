#include "Configuration.hpp"

int main() 
{
    ClearLogFile("../Logs/worker_log.txt");
    StartServer();
    return 0;
}
