#ifndef _CUBIE_H_
#define _CUBIE_H_

#include "cubie_config.h"
#include "glad/gl.h"
#include "quat.h"
#include "vertex.h"

#include <stdint.h>

typedef struct {
    Vertex *verts;
    uint64_t vertex_count;

    uint32_t *indices;
    uint64_t index_count;

    Quat ori;

    GLuint vao, vbo, ebo;
} Cubie;

Cubie cubie(Cubie_Config *cconf);
void cubie_free(Cubie c);

#endif // _CUBIE_H_