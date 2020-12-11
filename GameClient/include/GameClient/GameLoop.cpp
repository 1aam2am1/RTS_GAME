//
// Created by Michal_Marszalek on 09.12.2020.
//

#include "GameLoop.h"
#include <GameClient/Unity/Editor/EditorApplication.h>
#include <Core/Time.h>
#include <SceneManagement/SceneManager.h>
#include <Macro.h>
#include <Core/MonoBehaviour.h>

void GameLoop::run() {
    Time::m_unscaled_deltaTime = deltaClock.restart().asSeconds();

    if (EditorApplication::isPlaying != isPlaying) {

        //TODO: Load and save scene from playing mode to editor mode
        isPlaying = EditorApplication::isPlaying;

        if (isPlaying) {
            //TODO: From global settings
            Time::timeScale = 1;
            Time::maximumDeltaTime = 1.f / 5.f;
            Time::fixedDeltaTime = 0.02;
        }

        Time::m_Time = 0;
        Time::m_unscaled_Time = 0;
        Time::m_fixedTime = 0;
        Time::m_unscaled_fixedTime = 0;
        m_physics_time = 0;
    }

    if (!isPlaying) { return; }
    if (EditorApplication::isPaused) { return; }

    ///Timers
    {
        Time::m_deltaTime = Time::timeScale * Time::m_unscaled_deltaTime;

        Time::m_Time += Time::m_deltaTime;
        Time::m_unscaled_Time += Time::m_unscaled_deltaTime;

        m_physics_time += Time::m_deltaTime;
    }
    ///Start
    {
        for (auto &scene: SceneManager::data) {
            if (!scene.second.isLoaded) { continue; };

            for (auto &object : scene.second.new_components) {
                if (object) {
                    auto mono = dynamic_pointer_cast<MonoBehaviour>(object);
                    if (mono) {
                        if (mono->gameObject()->activeInHierarchy()) {
                            mono->Start();
                        } else { continue; }
                    }

                    scene.second.components.emplace_back(object);
                    object = nullptr;
                }
            }
            //TODO: Garbage collect new_components nullptr objects
        }
    }

    /// Physics
    while (true) {
        if (m_physics_time <= Time::fixedDeltaTime) { break; } //should run

        Time::m_fixedTime += Time::fixedDeltaTime;
        if (Time::timeScale != 0.0f) { Time::m_unscaled_fixedTime += Time::fixedDeltaTime / Time::timeScale; }

        m_physics_time -= Time::fixedDeltaTime;

        /**box2d*/
        /**should_run_every_time*/
        /**fixed update*/

        //If my elapsed frame time is bigger than should break.
        if (deltaClock.getElapsedTime().asSeconds() > Time::maximumDeltaTime) {
            m_physics_time = 0;
            break;
        }
    }

    /**
     * Events
     */

    ///Update
    for (auto &scene: SceneManager::data) {
        if (!scene.second.isLoaded) { continue; };

        for (auto &object : scene.second.components) {
            auto mono = dynamic_pointer_cast<MonoBehaviour>(object);
            if (mono && mono->isActiveAndEnabled()) {
                mono->Update();
            }
        }
    }

    /**
     * Render game scene to render texture
     */

    /**
     * Future onGui
     */
}
