#include "LoadBalancer.hpp"

unsigned int __stdcall RunLoadBalancer(void* param) {
    Queue* queue = ((RunLoadBalancerThreadParams*)param)->queue;

    while (1)
    {
        // Gracefully shutdown load balancer
        if (_kbhit() || (((RunLoadBalancerThreadParams*)param)->test_mode && QueueSize(queue) <= 0)) { // Check if a key has been pressed
            char ch = _getch(); // Get the pressed key
            if (ch == 'q' || ch == 'Q') {
                SetShutDown(((RunLoadBalancerThreadParams*)param)->queue);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down Load Balancer service...");
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down TCP handler service...");
                printf("\n[Intelligent Resource Manager]: Gracefully shutting down Bandwidth Statistics service...\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);   
                ExitThread(0);
            }
        }
    }

    return 0;
}