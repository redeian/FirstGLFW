#include "AudioManager.h"


AudioManager* AudioManager::instance;

AudioManager::AudioManager()
{
    /* Initialise ALUT and eat any ALUT-specific commandline flags. */
    if (!alutInit(0, NULL))
    {
        ALenum error = alutGetError();
        fprintf(stderr, "%s\n", alutGetErrorString(error));
        exit(EXIT_FAILURE);
    }
}


AudioManager::~AudioManager()
{

    alutExit();

    if (instance != nullptr) {
        delete instance;
    }
}

AudioManager & AudioManager::Get()
{
    if (instance == nullptr) {
        instance = new AudioManager();
    }

    return *instance;
}

void AudioManager::play(Audio audio)
{
    alSourcePlay(audio);
}

void AudioManager::pause( Audio audio)
{
    alSourcePause(audio);
}

void AudioManager::stop( Audio audio)
{
    alSourceStop(audio);
}

Audio AudioManager::LoadOrGetAudio(const std::string & fileName, const std::string& refName)
{
    if (fileName.empty())
        return 0;

    // find if the song is already exist;
    auto it = audioCollection.find(refName);
    if ( it != audioCollection.end()) {
        return it->second;
    }

    // if not exist, create a new one
    ALuint bufferID;
    ALuint audioSourceID;
    ALenum error;
    ALint status;

    /* Create an AL buffer from the given sound file. */
    bufferID = alutCreateBufferFromFile(fileName.c_str());
    if (bufferID == AL_NONE)
    {
        error = alutGetError();
        fprintf(stderr, "Error loading file: '%s'\n",
            alutGetErrorString(error));
        return 0;
    }

    /* Generate a single source, attach the buffer to it and start playing. */
    alGenSources(1, &audioSourceID);

    alSourcei(audioSourceID, AL_BUFFER, bufferID);


    // store in the collection
    audioCollection[refName] = (audioSourceID);

    return audioSourceID;
}
