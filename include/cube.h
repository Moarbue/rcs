#ifndef _CUBE_H_
#define _CUBE_H_

#include "cube_config.h"
#include "cubie.h"
#include "mat.h"
#include "shader.h"
#include "vec.h"

#include <stdint.h>

typedef struct {
    uint64_t w, h, d;

    // center cubies are not generated since they are not visible
    uint64_t cubie_count;
    Cubie *cubies;

    Vec3 origin;
    Quat ori;
    float scale;

    Shader_Program *prog;
} Rubiks_Cube;

Rubiks_Cube *rubiks_cube(Rubiks_Cube_Config *rcconf);
void rubiks_cube_draw(Rubiks_Cube *rc, Mat4 view_proj);
void rubiks_cube_free(Rubiks_Cube *rc);

#endif // _CUBE_H_