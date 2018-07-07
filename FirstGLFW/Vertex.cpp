#include "Vertex.h"



Vertex::Vertex()
{
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    pos.x = x;
    pos.y = y;
    pos.z = z;
    color.r = r;
    color.g = g;
    color.b = b;
}

Vertex::~Vertex()
{
}
