#pragma once

#include "thirdparty/glm/glm/glm.hpp"

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoor;
    glm::vec3 normal;

    Vertex();
    Vertex( float x, float y, float z, 
        float r, float g, float b, 
        float u, float v, 
        float normX, float normY, float normZ);
    ~Vertex();
};

