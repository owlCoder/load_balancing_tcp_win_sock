#include "GenerateMeasurementValue.hpp"

// Function to generate a random number in the range of 1 to 100,000 using XORShift128+
uint64_t xorshift128plus(struct RNGState* state) 
{
    uint64_t x = state->s[0];
    uint64_t const y = state->s[1];
    state->s[0] = y;
    x ^= x << 23; // Left shift
    state->s[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // Right shift and XOR

    return state->s[1] + y;
}