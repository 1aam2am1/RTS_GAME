//
// Created by Michal_Marszalek on 08.12.2020.
//

#ifndef RTS_GAME_EDITORAPPLICATION_H
#define RTS_GAME_EDITORAPPLICATION_H


class EditorApplication {
public:

    /// Is editor currently in play mode?
#if UNITY_EDITOR
    static bool isPlaying;
#else
    static constexpr bool isPlaying = true;
#endif

    /// Is editor currently paused?
#if UNITY_EDITOR
    static bool isPaused;
#else
    static constexpr bool isPaused = false;
#endif
};


#endif //RTS_GAME_EDITORAPPLICATION_H
