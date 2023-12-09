/*
   File: RNGState.hpp
   Description: Header file defining the RNGState structure for a random number generator.
*/

#ifndef RNG_STATE_H
#define RNG_STATE_H

#include <cstdint>

/*
  Structure: RNGState

  Represents the state of a random number generator.

  Members:
    s - An array of two 64-bit unsigned integers representing the state of the RNG.
*/
struct RNGState {
    uint64_t s[2];
};

#endif // RNG_STATE_H