/*
   File: GenerateMeasurementDate.hpp
   Description: Header file for GenerateMeasurementDate function.
*/

#ifndef GENERATE_MEASUREMENT_DATE_H
#define GENERATE_MEASUREMENT_DATE_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

/*
  Function: GenerateMeasurementDate

  Generates a date in the format DD.MM.YYYY and stores it in the provided 'date' string.

  Parameters:
    date - A character array to store the generated date string (format: DD.MM.YYYY)

  Returns:
    None
*/
void GenerateMeasurementDate(char* date);

#endif /* GENERATE_MEASUREMENT_DATE_H */
