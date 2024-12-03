#ifndef _CUBE_CONFIG_H_
#define _CUBE_CONFIG_H_

#include "color.h"
#include "cubie_config.h"

#include <stdint.h>

typedef struct {
    Color face_colors[CUBE_COLOR_COUNT];    // border color + 6 face colors

    // shader paths
    char *vertex_path;
    char *fragment_path;

    // cube dimensions in cubies
    uint64_t width, height, depth;

    float scale;    // scale of cube, 1 for no scaling (longest side has length 1)
    Vec3 origin;    // origin of the cube in the world

    float cubie_spacer_multiplier;  // air gap between cubies, relative to the length of a cubie [0..1]
    float face_length_multiplier;   // face length relative to cubie length [0...1]
    float face_offset_from_cubie;   // offset from cubie to face to prevent visual glitches, small value like 1e-3f should do
} Rubiks_Cube_Config;

#endif // _CUBE_CONFIG_H_