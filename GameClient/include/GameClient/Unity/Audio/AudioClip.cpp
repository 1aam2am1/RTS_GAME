//
// Created by Michal_Marszalek on 2021-01-19.
//

#include "AudioClip.h"
#include <GameClient/Unity/Macro.h>

EXPORT_CLASS(AudioClip)

int AudioClip::channels() {
    load();
    return buffer.getChannelCount();
}

int AudioClip::frequency() {
    load();
    return buffer.getSampleRate();
}

float AudioClip::length() {
    load();
    return buffer.getDuration().asSeconds();
}

void AudioClip::load() {
    if (!loaded) {
        loaded = true;
        buffer.loadFromFile(path);
    }
}
