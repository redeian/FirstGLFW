#pragma once
#include <map>
#include <memory>
#include "AL/alut.h"

typedef ALuint Audio;


class AudioManager final
{
private:

    std::map<std::string, Audio> audioCollection;
    static AudioManager* instance;

    AudioManager();
    ~AudioManager();

public:


    static AudioManager& Get();

    Audio LoadOrGetAudio(const std::string& fileName, const std::string& refName);
    void play(Audio audio);
    void pause(Audio audio);
    void stop(Audio audio);
    
};

