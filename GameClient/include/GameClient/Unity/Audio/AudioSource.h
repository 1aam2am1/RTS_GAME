//
// Created by Michal_Marszalek on 2021-01-19.
//

#ifndef RTS_GAME_AUDIOSOURCE_H
#define RTS_GAME_AUDIOSOURCE_H


#include <Core/Behaviour.h>
#include <SFML/Audio/Sound.hpp>
#include "AudioClip.h"

class AudioSource : public Behaviour {
public:
    AudioSource();

    /// The default AudioClip to play.
    TPtr<AudioClip> clip;

    /// If set to true, the audio source will automatically start playing on awake.
    bool playOnAwake = false;

    /// The volume of the audio source (0.0 to 1.0).
    SetterEmitterP<float, AudioSource> volume;

    /// Is the audio clip looping?
    SetterEmitterP<bool, AudioSource> loop;

    ///  Pauses playing the clip.
    void Pause();

    /// Plays the clip.
    void Play();

    /// Stops playing the clip.
    void Stop();

    /// Unpause the paused playback of this AudioSource.
    void UnPause();

protected:
    void Awake() override;

    void OnVolumeChange();

    void OnLoopChange();

    void OnEnable() override;

    void OnDisable() override;

    void Update() override;

private:
    sf::Sound playing;
    bool disabled = false;
};


#endif //RTS_GAME_AUDIOSOURCE_H
