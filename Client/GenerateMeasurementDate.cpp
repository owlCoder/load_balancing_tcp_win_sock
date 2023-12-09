#include "GenerateMeasurementDate.hpp"

void GenerateMeasurementDate(char* date)
{
    int year = rand() % (2023 - 2000 + 1) + 2000;
    int month = rand() % 12 + 1;
    int day;

    switch (month) {
    case 2: // February
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            day = rand() % 29 + 1; // Leap year
        }
        else {
            day = rand() % 28 + 1; // Non-leap year
        }
        break;
    case 4: case 6: case 9: case 11: // April, June, September, November
        day = rand() % 30 + 1;
        break;
    default:
        day = rand() % 31 + 1;
    }

    sprintf(date, "%02d.%02d.%04d", day, month, year);
}