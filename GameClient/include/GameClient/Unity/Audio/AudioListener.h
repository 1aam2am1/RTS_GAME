//
// Created by Michal_Marszalek on 2021-01-19.
//

#ifndef RTS_GAME_AUDIOLISTENER_H
#define RTS_GAME_AUDIOLISTENER_H


#include <Core/Behaviour.h>
#include <SFML/Audio/Listener.hpp>

class AudioListener : public Behaviour {
public:
    /// The paused state of the audio system.
    static bool pause;

    ///  Controls the game sound volume (0.0 to 1.0).
    static float volume;

protected:
    void OnEnable() override;

    void OnDisable() override;

    void OnDestroy() override;

    void Update() override;
};


#endif //RTS_GAME_AUDIOLISTENER_H
