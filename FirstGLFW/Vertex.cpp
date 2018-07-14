#include "Vertex.h"



Vertex::Vertex()
{
}

Vertex::Vertex(float x, float y, float z,
    float r, float g, float b,
    float u, float v,
    float normX, float normY,float normZ)
{
    pos.x = x;
    pos.y = y;
    pos.z = z;
    color.r = r;
    color.g = g;
    color.b = b;
    texCoor.x = u;
    texCoor.y = v;
    normal.x = normX;
    normal.y = normY;
}

Vertex::~Vertex()
{
}
