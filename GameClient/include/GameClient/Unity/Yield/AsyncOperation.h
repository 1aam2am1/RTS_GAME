//
// Created by Michal_Marszalek on 14.09.2020.
//

#ifndef RTS_GAME_ASYNCOPERATION_H
#define RTS_GAME_ASYNCOPERATION_H

#include <Yield/YieldInstruction.h>
#include <GameApi/Signal.h>

/// Asynchronous operation coroutine.
/// \note You can yield until asynchronous operation continues, or manually check whether it's done (isDone) or progress (progress).
class AsyncOperation : public YieldInstruction {
public:

    /// Has the operation finished? (Read Only)
    /// \note Flipping to true can be delayed.
    /// \see allowSceneActivation
    const bool &isDone = m_isDone;

    ///What's the operation's progress. (Read Only)
    /// \note Return an operation's progress. (Read Only) This returns how close the operation is to finishing.
    /// The operation is finished when the progress float reaches 1.0 and isDone is called.
    /// If you set allowSceneActivation to false, progress is halted at 0.9 until it is set to true. This is extremely useful for creating loading bars.
    const float &progress = m_progress;

    /// Priority lets you tweak in which order async operation calls will be performed.
    /// \note When multiple asynchronous operations are queued up, the operation with the higher priority will be executed first.
    /// Once an operation has been started on the background thread, changing the priority will have no effect anymore.
    int priority;

    /// Allow Scenes to be activated as soon as it is ready.
    /// \details When used in combination with LoadLevelAsync & LoadLevelAdditiveAsync it allows you to delay the actual activation of the Scene.
    /// (And unloading of the previous Scene).
    /// \note When allowSceneActivation is set to false then progress is stopped at 0.9. The isDone is then maintained at false.
    /// When allowSceneActivation is set to true isDone can complete. While isDone is false, the AsyncOperation queue is stalled.
    /// For example, if a LoadSceneAsync.allowSceneActivation is set to false, and another AsyncOperation (e.g. SceneManager.UnloadSceneAsync )
    /// is initialized, the last operation will not be called before the first allowSceneActivation is set to true.
    bool allowSceneActivation;

    /// Event that is invoked upon operation completion. An event handler that is registered in the same frame as the call
    /// that creates it will be invoked next frame, even if the operation is able to complete synchronously.
    /// If a handler is registered after the operation has completed and has already invoked the complete event,
    /// the handler will be called synchronously.
    sigslot::signal<AsyncOperation> completed;

protected:
    bool m_isDone = false;
    float m_progress = 0.f;

    YieldReturn operator()() override;
};


#endif //RTS_GAME_ASYNCOPERATION_H
