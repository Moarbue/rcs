#ifndef _SMATH_H_
#define _SMATH_H_

#include "vec.h"
#include "mat.h"
#include "quat.h"

#define _USE_MATH_DEFINES
#include <math.h>

// smallest difference we care for
#define EPS 1e-6

float rad(float deg);
float deg(float rad);

float pow2(float s);

float min(float a, float b);
float max(float a, float b);
float clamp(float val, float minVal, float maxVal);

int imin(int a, int b);
int imax(int a, int b);
int iclamp(int val, int minVal, int maxVal);

float lerp(float a, float b, float t);

#endif // _SMATH_H_