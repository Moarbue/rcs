#include "vertex.h"

Vertex vertex(Vec3 pos, Color col)
{
    return (Vertex) {pos, col};
}