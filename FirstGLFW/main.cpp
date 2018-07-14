#include <iostream>


#include "AL/alut.h"
#include "App.h"

#include "AudioManager.h"

int main() {

    // bg song
    auto a1 = AudioManager::Get().LoadOrGetAudio("zelda.wav", "bg");
    // sound effect
    auto a2 = AudioManager::Get().LoadOrGetAudio("trucks001.wav", "effect1");
    // same sound effect but different audio object
    auto a3 = AudioManager::Get().LoadOrGetAudio("trucks001.wav", "effect2");

    AudioManager::Get().play(a3);
    
    alutSleep(1.0f);

    AudioManager::Get().play(a2);

    alutSleep(1.0f);

    AudioManager::Get().play(a1);

    App app;
   
    app.init( 640, 480, "My Game" );
    app.Start();

    if (!alutExit())
    {
        ALenum error = alutGetError();
        fprintf(stderr, "%s\n", alutGetErrorString(error));
        exit(EXIT_FAILURE);
    }
    
    return 0;
}