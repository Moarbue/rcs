#include "smath.h"

// Convert degrees to radians
float rad(float deg)
{
    return deg * (float)M_PI / 180.0f;
}

// Convert radians to degrees
float deg(float rad)
{
    return rad * 180.0f * (float)M_1_PI;
}

// Compute the square of a float
float pow2(float s)
{
    return s * s;
}

// Compute the minimum of a and b
float min(float a, float b)
{
    if (a < b) return a;
    return b;
}

// Compute the maximum of a and b
float max(float a, float b)
{
    if (a > b) return a;
    return b;
}

// Clamp val from minVal to maxVal
float clamp(float val, float minVal, float maxVal)
{
    return min(max(val, minVal), maxVal);
}

// Same as min but for int
int imin(int a, int b)
{
    if (a < b) return a;
    return b;
}

// Same as max but for int
int imax(int a, int b)
{
    if (a > b) return a;
    return b;
}

// Same as clamp but for int
int iclamp(int val, int minVal, int maxVal)
{
    return min(max(val, minVal), maxVal);
}

// same as min but for uint64_t
uint64_t u64min(uint64_t a, uint64_t b)
{
    if (a < b) return a;
    return b;
}

// same as max but for uint64_t
uint64_t u64max(uint64_t a, uint64_t b)
{
    if (a > b) return a;
    return b;
}

// Linear interpolation from a to b
float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}