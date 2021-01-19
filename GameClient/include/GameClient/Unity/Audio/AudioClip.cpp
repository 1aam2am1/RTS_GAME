//
// Created by Michal_Marszalek on 2021-01-19.
//

#include "AudioClip.h"
#include <GameClient/Unity/Macro.h>

EXPORT_CLASS(AudioClip)

int AudioClip::channels() {
    return buffer.getChannelCount();
}

int AudioClip::frequency() {
    return buffer.getSampleRate();
}

float AudioClip::length() {
    return buffer.getDuration().asSeconds();
}
