#include "vertex.h"

Cube_Vertex cube_vertex(Vec3 pos, Color col)
{
    return (Cube_Vertex) {pos, col};
}

Font_Vertex font_vertex(Vec2 pos, Vec2 tex)
{
    return (Font_Vertex) {pos, tex};
}