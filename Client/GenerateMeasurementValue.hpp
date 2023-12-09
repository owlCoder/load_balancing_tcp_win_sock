/*
   File: GenerateMeasurementValue.hpp
   Description: Header file containing the XORShift128+ function declaration.
*/

#ifndef GENERATE_MEASUREMENT_VALUE_H
#define GENERATE_MEASUREMENT_VALUE_H

#include "../Common/RNGState.hpp"

/*
  Function: xorshift128plus

  Generates a 64-bit random number using the XORShift128+ algorithm based on the given state.

  Parameters:
    state - A pointer to an RNGState struct representing the state of the random number generator.

  Returns:
    uint64_t - A 64-bit unsigned random number generated using the XORShift128+ algorithm.
*/
uint64_t xorshift128plus(struct RNGState* state);

#endif // GENERATE_MEASUREMENT_VALUE_H
