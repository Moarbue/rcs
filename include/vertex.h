#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "color.h"
#include "vec.h"

typedef enum {
    CUBE_VERTEX_POS,
    CUBE_VERTEX_COL,
} Cube_Vertex_Attributes;

typedef struct {
    Vec3  pos;
    Color col;
} Cube_Vertex;

Cube_Vertex cube_vertex(Vec3 pos, Color col);

typedef enum {
    FONT_VERTEX_POS,
    FONT_VERTEX_TEX,
} Font_Vertex_Attributes;

typedef struct {
    Vec2 pos;
    Vec2 tex;
} Font_Vertex;

Font_Vertex font_vertex(Vec2 pos, Vec2 tex);

#endif // _VERTEX_H_