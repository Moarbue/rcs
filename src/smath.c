#include "smath.h"

#define _USE_MATH_DEFINES
#include <math.h>

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

// Linear interpolation from a to b
float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}