#include <iostream>


#include "App.h"


int main() {

    App app;
   
    app.init( 640, 480, "My Game" );
    app.Start();
   
    return 0;
}