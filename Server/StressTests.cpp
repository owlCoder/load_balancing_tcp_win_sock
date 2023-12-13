#include "StressTests.hpp"

bool RunQueueCapacityTest(const int max_allocations) 
{
    struct Queue queue;
    InitializeQueue(&queue);

    const int logFrequency = 10000; // Log memory usage after every 10 000 allocations

    int allocationsCount = 0;
    int deallocationsCount = 0;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    
    for (int i = 0; i < max_allocations; ++i) {
        struct MeasurementData* data = (struct MeasurementData*)malloc(sizeof(struct MeasurementData));
        if (data == NULL) {
            // Handle allocation failure
            continue;
        }

        Enqueue(&queue, data);
        allocationsCount++;

        if (allocationsCount % logFrequency == 0) {
            printf("[Queue Allocator]:   Memory allocated after %7d allocations:   %10lld bytes\n", allocationsCount, allocationsCount * (long long) sizeof(struct MeasurementData));
        }
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    while (QueueSize(&queue) > 0) {
        struct MeasurementData* data = Dequeue(&queue);
        free(data);
        deallocationsCount++;

        if (deallocationsCount % logFrequency == 0) {
            printf("[Queue Deallocator]: Memory freed up after %7d  deallocations: %10lld bytes\n", deallocationsCount, deallocationsCount * (long long) sizeof(struct MeasurementData));
        }
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    DestroyQueue(&queue);

    if (allocationsCount != deallocationsCount) {
        return false; // not all allocations has been made successfull
    }
    else {
        return true;
    }

    return 0;
}