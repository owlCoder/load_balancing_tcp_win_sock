/*
   File: MeasurementData.hpp
   Description: Header file defining the MeasurementData structure.
*/

#ifndef MEASUREMENT_DATA_H
#define MEASUREMENT_DATA_H

/*
  Structure: MeasurementData

  Represents measurement data consisting of a date and a measurement value.

  Members:
    date - A character array to store the date in DD.MM.YYYY format (+1 for null terminator).
    measurementValue - An unsigned integer representing the measurement value.
*/
typedef struct MeasurementData {
    char date[11];
    unsigned int measurementValue;
} MeasurementData;

#endif /* MEASUREMENT_DATA_H */
