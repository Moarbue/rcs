#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "color.h"
#include "vec.h"

typedef enum {
    VERTEX_POS,
    VERTEX_COL,
} Vertex_Attributes;

typedef struct {
    Vec3  pos;
    Color col;
} Vertex;

Vertex vertex(Vec3 pos, Color col);

#endif // _VERTEX_H_