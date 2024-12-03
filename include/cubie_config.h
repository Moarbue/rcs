#ifndef _CUBIE_CONFIG_H_
#define _CUBIE_CONFIG_H_

#include "color.h"
#include "vec.h"

#include <stdint.h>

// mask to specify which faces of cubie are colored
typedef enum {
    COLOR_MASK_FRONT =  1,
    COLOR_MASK_UP    =  2,
    COLOR_MASK_LEFT  =  4,
    COLOR_MASK_BACK  =  8,
    COLOR_MASK_DOWN  = 16,
    COLOR_MASK_RIGHT = 32,

    COLOR_MASK_COUNT =  6,
} Cube_Color_Mask;

typedef enum {
    COLOR_BORDER,
    COLOR_FRONT,
    COLOR_UP,
    COLOR_LEFT,
    COLOR_BACK,
    COLOR_DOWN,
    COLOR_RIGHT,

    CUBE_COLOR_COUNT,
} Cube_Color;

typedef struct {
    Vec3 origin;        // origin of cubie in world, top left corner, coordinates are offset directly by origin instead of a translation matrix
    float side_length;  // side length of cubie

    uint8_t color_mask; // mask to determine the colored faces of a cubie, combination of Cube_Color_Mask enum

    Color face_colors[CUBE_COLOR_COUNT];    // border color + 6 face colors
    float face_length_multiplier;           // face length relative to cubie length [0...1]
    float face_offset_from_cubie;           // offset from cubie to face to prevent visual glitches, small value like 1e-3f should do
} Cubie_Config;

#endif // _CUBIE_CONFIG_H_