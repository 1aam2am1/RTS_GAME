//
// Created by Michal_Marszalek on 09.12.2020.
//

#ifndef RTS_GAME_TIME_H
#define RTS_GAME_TIME_H

#include <GameApi/SetterGetter.h>

class Time {
public:

    /// The scale at which time passes. This can be used for slow motion effects.
    static float timeScale;

    /// The maximum time a frame can take
    static float maximumDeltaTime;

    /// The completion time in seconds since the last frame
    /// \details This property provides the time between the current and previous frame.
    /// Use Time.deltaTime to move a GameObject in the y direction, at n units per second.
    /// Multiply n by Time.deltaTime and add to the y component.
    [[nodiscard]] static float deltaTime();

    /// The timeScale-independent interval in seconds from the last frame to the current one
    [[nodiscard]] static float unscaledDeltaTime();

    /// The interval in seconds at which physics and other
    /// fixed frame rate updates (like MonoBehaviour's FixedUpdate) are performed.
    /// \note Note that the fixedDeltaTime interval is with respect to the in-game time affected by timeScale.
    static float fixedDeltaTime;

    /// The timeScale-independent interval in seconds from the last fixed frame to the current one
    [[nodiscard]] static float fixedUnscaledDeltaTime();

    /// This is the time in seconds since the start of the game.
    [[nodiscard]] static float time();

    /// The timeScale-independant time for this frame.
    /// This is the time in seconds since the start of the game.
    [[nodiscard]] static float unscaledTime();

    /// The time the latest FixedUpdate has started
    [[nodiscard]] static float fixedTime();

    /// The TimeScale-independant time the latest FixedUpdate has started.
    /// This is the time in seconds since the start of the game.
    [[nodiscard]] static float fixedUnscaledTime();

private:
    friend class GameLoop;

    static float m_Time;               ///< From start of game
    static float m_unscaled_Time;      ///< From start of game, without scale

    static float m_fixedTime;          ///< Fixed from start of game
    static float m_unscaled_fixedTime; ///< Fixed from start of game, without scale

    static float m_deltaTime;          ///< From last frame
    static float m_unscaled_deltaTime; ///< From last frame, without scale
};


#endif //RTS_GAME_TIME_H
