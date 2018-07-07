#pragma once

#include "thirdparty/glm/glm/glm.hpp"

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;

    Vertex();
    Vertex( float x, float y, float z, float r, float g, float b);
    ~Vertex();
};

