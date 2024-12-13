#ifndef _CUBIE_H_
#define _CUBIE_H_

#include "animation.h"
#include "cubie_config.h"
#include "glad/gl.h"
#include "quat.h"
#include "vec.h"
#include "vertex.h"

#include <stdint.h>

typedef struct {
    Vertex *verts;
    uint64_t vertex_count;

    uint32_t *indices;
    uint64_t index_count;

    Quat ori;
    Animation a;

    GLuint vao, vbo, ebo;
} Cubie;

Cubie cubie(Cubie_Config *cconf);
void cubie_set_animation_duration(Cubie *c, float duration);
void cubie_set_animation_easing_func(Cubie *c, easing_func efunc);
void cubie_rotation_add(Cubie *c, Vec3 axis, float angle);
void cubie_update(Cubie *c, float dt);
void cubie_free(Cubie c);

#endif // _CUBIE_H_