#ifndef _CUBE_H_
#define _CUBE_H_

#include "animation.h"
#include "cube_config.h"
#include "cubie.h"
#include "mat.h"
#include "shader.h"
#include "vec.h"

#include <stdint.h>

typedef enum {
    FACE_FRONT,
    FACE_UP,
    FACE_LEFT,
    FACE_BACK,
    FACE_DOWN,
    FACE_RIGHT,
} Rubiks_Cube_Face;

typedef enum {
    ROTATION_CCW,
    ROTATION_180,
    ROTATION_CW,
} Rubiks_Cube_Rotation;

typedef struct {
    uint64_t w, h, d;

    uint64_t max_length;
    // cubie indices if center cubies were not removed
    uint64_t *cubie_indices;

    // center cubies are not generated since they are not visible
    uint64_t cubie_count;
    Cubie *cubies;

    // move cooldown
    float mcooldown;
    float mc;

    Vec3 pos;
    Quat ori;
    float scale;

    Shader_Program *prog;
} Rubiks_Cube;

Rubiks_Cube *rubiks_cube(Rubiks_Cube_Config *rcconf);
void rubiks_cube_set_move_duration(Rubiks_Cube *rc, float duration);
void rubiks_cube_set_move_cooldownn(Rubiks_Cube *rc, float cooldown);
void rubiks_cube_set_move_easing_func(Rubiks_Cube *rc, easing_func efunc);
void rubiks_cube_rotate_slice(Rubiks_Cube *rc, Rubiks_Cube_Face face, Rubiks_Cube_Rotation rot, uint64_t slice);
void rubiks_cube_update(Rubiks_Cube *rc, float dt);
void rubiks_cube_draw(Rubiks_Cube *rc, Mat4 view_proj);
void rubiks_cube_free(Rubiks_Cube *rc);

#endif // _CUBE_H_