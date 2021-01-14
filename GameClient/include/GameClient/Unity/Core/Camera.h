//
// Created by Michal_Marszalek on 14.12.2020.
//

#ifndef RTS_GAME_CAMERA_H
#define RTS_GAME_CAMERA_H


#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Behaviour.h"
#include <map>

class Camera : public Behaviour {
public:
    /// Returns all enabled cameras in the Scene.
    static std::vector<TPtr<Camera>> allCameras();

    /// The first enabled Camera component that is tagged "MainCamera"
    static TPtr<Camera> main();

    /// Delegate that you can use to execute custom code before a Camera culls the scene.
    static sigslot::signal<TPtr<Camera>> onPreCull;

    /// Delegate that you can use to execute custom code before a Camera renders the scene.
    static sigslot::signal<TPtr<Camera>> onPreRender;

    /// Delegate that you can use to execute custom code after a Camera renders the scene.
    static sigslot::signal<TPtr<Camera>> onPostRender;

    Camera();

    ~Camera() override;

    /// The color with which the screen will be cleared.
    sf::Color backgroundColor = sf::Color::Black;

    /// Get the world-space speed of the camera
    sf::Vector3f velocity();

    /// Camera's half-size when in orthographic mode.
    /// \note this means that if it is 1 then object that have 1m is half of screen
    double orthographicSize = 3.6f;

    /// Set the target display for this Camera.
    /// \note This setting makes a Camera render into the specified display.
    /// Maximum number of displays (eg. monitors) supported is 8.
    //int targetDisplay = 0;

    /// Where on the screen is the camera rendered in screen coordinates.
    sf::FloatRect pixelRect = {0.f, 0.f, 1.f, 1.f};

    /// Camera's depth in the camera rendering order.
    /// \note Cameras with lower depth are rendered before cameras with higher depth.
    SetterGetter<double> depth;

    /// Render the camera manually.
    void Render();

private:
    float m_depth = 0;

    void Awake() override;
};


#endif //RTS_GAME_CAMERA_H
