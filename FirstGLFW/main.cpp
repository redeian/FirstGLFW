#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "thirdparty/glm/glm/glm.hpp"

#include "App.h"

using namespace glm;

int main() {

    App app;
   
    app.init( 640, 480, "My Game" );
    app.Start();
   
    return 0;
}