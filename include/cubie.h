#ifndef _CUBIE_H_
#define _CUBIE_H_

#include "glad/gl.h"
#include "quat.h"
#include "vertex.h"

#include <stdint.h>

typedef enum {
    COLOR_MASK_FRONT =  1,
    COLOR_MASK_UP    =  2,
    COLOR_MASK_LEFT  =  4,
    COLOR_MASK_BACK  =  8,
    COLOR_MASK_DOWN  = 16,
    COLOR_MASK_RIGHT = 32,

    COLOR_MASK_COUNT =  6,
} Cube_Color_Mask;

typedef struct {
    Vertex *verts;
    uint64_t vertex_count;

    uint32_t *indices;
    uint64_t index_count;

    Quat ori;

    GLuint vao, vbo, ebo;
} Cubie;

Cubie cubie(Vec3 origin, float side_length, float face_length, float face_offset_from_cube, uint8_t color_mask);

#endif // _CUBIE_H_