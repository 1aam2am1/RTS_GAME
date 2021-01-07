//
// Created by Michal_Marszalek on 09.09.2020.
//

#ifndef RTS_GAME_MONOBEHAVIOUR_H
#define RTS_GAME_MONOBEHAVIOUR_H

#include <functional>
#include <GameClient/Unity/Yield/Coroutine.h>
#include <GameClient/Unity/Physics2D/Collider2D.h>
#include <GameClient/Unity/Physics2D/Joint2D.h>
#include <GameClient/Unity/Physics2D/Collision2D.h>
#include "Behaviour.h"

/// MonoBehaviour is the base class from which every Unity script derives.
class MonoBehaviour : public Behaviour {
public:

    /// Starts a Coroutine.
    /// \details Normal coroutine updates are run after the Update function returns.
    Coroutine StartCoroutine(std::function<YieldReturn()> routine);

    /// Stops all coroutines running on this behaviour.
    void StopAllCoroutines();

    /// Stops the first coroutine named methodName, or the coroutine stored in routine running on this behaviour.
    void StopCoroutine(Coroutine routine);

    /// Awake is called when the script instance is being loaded.
    virtual void Awake();

    /// Frame-rate independent MonoBehaviour.FixedUpdate message for physics calculations.
    virtual void FixedUpdate();

    /// LateUpdate is called every frame, if the Behaviour is enabled.
    /// \details LateUpdate is called after all Update functions have been called.
    /// This is useful to order script execution. For example a follow camera should always be
    /// implemented in LateUpdate because it tracks objects that might have moved inside Update.
    virtual void LateUpdate();

    /// Sent to all GameObjects when the player gets or loses focus.
    /// \param hasFocus True if the GameObjects have focus, else False.
    virtual void OnApplicationFocus(bool hasFocus);

    /// Sent to all GameObjects when the application pauses.
    /// \param pauseStatus True if the application is paused, else False.
    virtual void OnApplicationPause(bool pauseStatus);

    /// Sent to all game objects before the application quits.
    virtual void OnApplicationQuit();

    /// OnBecameInvisible is called when the renderer is no longer visible by any camera.
    virtual void OnBecameInvisible();

    /// OnBecameVisible is called when the renderer became visible by any camera.
    virtual void OnBecameVisible();

    /// Sent when an incoming collider makes contact with this object's collider (2D physics only).
    /// \param other The Collision2D data associated with this collision.
    virtual void OnCollisionEnter2D(const Collision2D &other);

    /// Sent when a collider on another object stops touching this object's collider (2D physics only).
    /// \param other The Collision2D data associated with this collision.
    virtual void OnCollisionExit2D(const Collision2D &other);

    /// Sent each frame where a collider on another object is touching this object's collider (2D physics only).
    /// \param other The Collision2D data associated with this collision.
    virtual void OnCollisionStay2D(const Collision2D &other);

    /// Destroying the attached Behaviour will result in the game or Scene receiving OnDestroy.
    /// \details OnDestroy will only be called on game objects that have previously been active.
    virtual void OnDestroy();

    /// This function is called when the behaviour becomes disabled.
    virtual void OnDisable();

    /// Implement OnDrawGizmos if you want to draw gizmos that are also pickable and always drawn.
    virtual void OnDrawGizmos();

    /// Implement OnDrawGizmosSelected to draw a gizmo if the object is selected.
    virtual void OnDrawGizmosSelected();

    /// This function is called when the object becomes enabled and active.
    virtual void OnEnable();

    /// Called when a Joint2D attached to the same game object breaks.
    /// \details After OnJointBreak2D is called, the joint will automatically be removed from the game object and deleted.
    virtual void OnJointBreak2D(const Joint2D &brokenJoint);

    /// OnMouseDown is called when the user has pressed the mouse button while over the Collider.
    virtual void OnMouseDown();

    /// OnMouseDrag is called when the user has clicked on a Collider and is still holding down the mouse.
    virtual void OnMouseDrag();

    /// Called when the mouse enters the Collider.
    virtual void OnMouseEnter();

    /// Called when the mouse is not any longer over the Collider.
    virtual void OnMouseExit();

    /// Called every frame while the mouse is over the Collider.
    virtual void OnMouseOver();

    /// OnMouseUp is called when the user has released the mouse button.
    virtual void OnMouseUp();

    /// OnMouseUpAsButton is only called when the mouse is released over the same Collider as it was pressed.
    virtual void OnMouseUpAsButton();

    /// This function is called when the list of children of the transform of the GameObject has changed.
    virtual void OnTransformChildrenChanged();

    /// This function is called when the parent property of the transform of the GameObject has changed.
    virtual void OnTransformParentChanged();

    /// Sent when another object enters a trigger collider attached to this object (2D physics only).
    /// \param other The other Collider2D involved in this collision.
    virtual void OnTriggerEnter2D(const Collider2D &other);

    /// Sent when another object leaves a trigger collider attached to this object (2D physics only).
    /// \param other The other Collider2D involved in this collision.
    virtual void OnTriggerExit2D(const Collider2D &other);

    /// Sent each frame where another object is within a trigger collider attached to this object (2D physics only).
    /// \param other The other Collider2D involved in this collision.
    virtual void OnTriggerStay2D(const Collider2D &other);

    /// This function is called when the script is loaded or a value is changed in the Inspector (Called in the editor only).
    /// \details You can use this to ensure that when you modify data in an editor,
    /// that data stays within a certain range.
    virtual void OnValidate();

    /// Start is called on the frame when a script is enabled just before any of the Update methods are called the first time.
    virtual void Start();

    /// Update is called every frame, if the MonoBehaviour is enabled.
    virtual void Update();

private:
    friend class GameObject;

    bool was_awakened = false;

    void UnityAwake() override {
        if (!was_awakened) {
            was_awakened = true;
            Awake();
        } else {
            GameApi::log(ERR << "It should not happen fix please");
        }
    }
};


#endif //RTS_GAME_MONOBEHAVIOUR_H
