//
// Created by Michal_Marszalek on 2021-01-19.
//

#ifndef RTS_GAME_AUDIOCLIP_H
#define RTS_GAME_AUDIOCLIP_H


#include <Core/Object.h>
#include <SFML/Audio/SoundBuffer.hpp>

class AudioClip : public Object {
public:

    /// The number of channels in the audio clip.
    int channels();

    /// The sample frequency of the clip in Hertz.
    int frequency();

    /// The length of the audio clip in seconds.
    float length();

private:
    friend class AudioSource;

    friend class AudioImporter;

    sf::SoundBuffer buffer;

    void load();

    bool loaded = false;

    std::string path;
};


#endif //RTS_GAME_AUDIOCLIP_H
