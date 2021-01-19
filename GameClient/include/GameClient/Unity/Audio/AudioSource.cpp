//
// Created by Michal_Marszalek on 2021-01-19.
//

#include "AudioSource.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_COMPONENT_MENU(AudioSource, clip, playOnAwake, volume, loop)

AudioSource::AudioSource() : volume(this, &AudioSource::OnVolumeChange, 1.0f),
                             loop(this, &AudioSource::OnLoopChange, false) {

}

void AudioSource::Pause() {
    playing.pause();
}

void AudioSource::Play() {
    playing.stop();

    if (!clip) { return; }
    if (!isActiveAndEnabled()) { return; }

    playing.setBuffer(clip->buffer);
    playing.play();
}

void AudioSource::Stop() {
    playing.stop();
}

void AudioSource::UnPause() {
    if (!isActiveAndEnabled()) { return; }
    playing.play();
}

void AudioSource::Awake() {
    if (playOnAwake) {
        playing.setBuffer(clip->buffer);
        disabled = true;
    }
}

void AudioSource::OnVolumeChange() {
    volume.t = std::clamp(volume.get(), 0.f, 1.f);
    playing.setVolume(volume * 100.f);
}

void AudioSource::OnLoopChange() {
    playing.setLoop(loop);
}

void AudioSource::OnEnable() {
    if (disabled) {
        disabled = false;
        playing.play();
    }
}

void AudioSource::OnDisable() {
    if (playing.getStatus() == sf::SoundSource::Playing) {
        disabled = true;
        playing.pause();
    }
}

void AudioSource::Update() {
    auto p = transform()->localPosition.get();
    p.z = 0;
    playing.setPosition(p);
}
