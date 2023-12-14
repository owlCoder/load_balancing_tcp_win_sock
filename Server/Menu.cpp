#include "Menu.hpp"

void DisplayMenu() {
    printf("\n=============================================\n");
    printf("                SERVER MENU\n");
    printf("=============================================\n");
    printf("1 - Run Queue Stress Test\n");
    printf("2 - Run Bandwidth Stress Test\n");
    printf("3 - Run Server\n");
    printf("4 - Exit\n");
    printf("=============================================\n");
}

void RunDesiredOption(void (*serverFunction)()) {
    int choice;
    do {
        DisplayMenu();
        printf("Enter your choice: ");
        if (scanf_s("%d", &choice) != 1) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("\n[Option Validator]: Invalid choice! Please enter a valid option.\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        printf("\n");

        switch (choice) {
        case 1:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("-------------------------------------------------------------------------------------------\n");
            printf("[Option Runner]: Running Queue Stress Test...\n");
            printf("-------------------------------------------------------------------------------------------\n\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            // Function to run Queue Stress Test
            if (RunQueueCapacityTest(100000, 10000)) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
                printf("\n=========================== [Stress Test Checker]: Test Passed ============================\n");
            }
            else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
                printf("\n=========================== [Stress Test Checker]: Test Failed ===========================\n");
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("\n-------------------------------------------------------------------------------------------");
            printf("\n[Option Runner]: Running Queue Stress Test Completed\n");
            printf("-------------------------------------------------------------------------------------------\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        case 2:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("-------------------------------------------------------------------------------------------\n");
            printf("[Option Runner]: Running Bandwidth Stress Test...\n");
            printf("-------------------------------------------------------------------------------------------\n\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            // Run server run clients simulate load balancer activity

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("\n-------------------------------------------------------------------------------------------");
            printf("\n[Option Runner]: Running Bandwidth Stress Test Completed\n");
            printf("-------------------------------------------------------------------------------------------\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        case 3:
            system("cls");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
            printf("[Option Runner]: Running Server...\n");
            // Function to run Server
            ClearLogFile("../Logs/worker_log.txt");
            serverFunction();
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("\n[Option Runner]: Server has been shutted down\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            break;
        case 4:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("[Option Runner]: Exiting...\n\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("Press any key to close server console...");
            int c;
            c = getchar();
            exit(0);
            break;
        default:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("[Option Validator]: Invalid choice! Please enter a valid option.\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
    } while (choice != 4);
}