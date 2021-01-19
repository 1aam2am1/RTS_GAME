//
// Created by Michal_Marszalek on 24.11.2020.
//

#ifndef RTS_GAME_APPLICATION_H
#define RTS_GAME_APPLICATION_H


class Application {
public:

    /// Returns true when called in any kind of built Player, or when called in the Editor in Play Mode (Read Only)
#if UNITY_EDITOR

    static bool isPlaying();

#else
    static constexpr bool isPlaying() { return true; }
#endif

    /// Are we running inside the Unity editor? (Read Only)
    /// Returns true if the game is being run from the Unity editor; false if run from any deployment target.
#if UNITY_EDITOR
    static constexpr bool isEditor = true;
#else
    static constexpr bool isEditor = false;
#endif

    static void Quit();
};


#endif //RTS_GAME_APPLICATION_H
