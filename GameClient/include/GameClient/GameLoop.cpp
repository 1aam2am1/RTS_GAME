//
// Created by Michal_Marszalek on 09.12.2020.
//

#include "GameLoop.h"
#include <GameClient/GlobalStaticVariables.h>
#include <GameClient/Unity/Editor/EditorApplication.h>
#include <Core/Time.h>
#include <SceneManagement/EditorSceneManager.h>
#include <Core/MonoBehaviour.h>
#include <Core/Attributes.h>
#include <Editor/AssetDatabase.h>

#if UNITY_EDITOR
extern bool Application_isPlaying;
#endif

void GameLoop::run() {
    Time::m_unscaled_deltaTime = deltaClock.restart().asSeconds();

#if UNITY_EDITOR
    if (EditorApplication::isPlaying != isPlaying) {

        isPlaying = EditorApplication::isPlaying;

        //TODO: -- Change saving directory, save id of editing gameobject to try resume editor settings before and after play
        Application_isPlaying = EditorApplication::isPlaying && !EditorApplication::isPaused;
        if (isPlaying) {
            //load default scene or active one
            if (!SceneManager::GetActiveScene()->isValid()) {
                EditorApplication::isPlaying = false;
                isPlaying = false;
            } else {

                loaded_scene_path = global.scene.data[global.scene.active_scene].path;
                if (AssetDatabase::GetAssetPath(EditorSceneManager::playModeStartScene.get()).empty()) {
                    if (!loaded_scene_path.empty()) {
                        EditorSceneManager::SaveScene(SceneManager::GetActiveScene());
                    } else {
                        loaded_scene_path = "Assets/_U.unity";
                        EditorSceneManager::SaveScene(SceneManager::GetActiveScene(), loaded_scene_path);
                    }
                } else {
                    EditorSceneManager::SaveScene(SceneManager::GetActiveScene());
                    loaded_scene_path = AssetDatabase::GetAssetPath(EditorSceneManager::playModeStartScene.get());
                }


                EditorSceneManager::OpenScene(loaded_scene_path);

                global.settings.Apply();
            }
        } else {
            //Load old scene
            EditorSceneManager::OpenScene(loaded_scene_path);
            if (global.scene.data[global.scene.active_scene].path == "Assets/_U.unity") {
                global.scene.data[global.scene.active_scene].path.clear();
                global.scene.data[global.scene.active_scene].name = "Scene";
                fs::remove("Assets/_U.unity"); //Clear after yourself
                fs::remove("Assets/_U.unity.meta");
            }
        }

        Time::m_Time = 0;
        Time::m_unscaled_Time = 0;
        Time::m_fixedTime = 0;
        Time::m_unscaled_fixedTime = 0;
        m_physics_time = 0;
    }
    Application_isPlaying = EditorApplication::isPlaying && !EditorApplication::isPaused;
#else
    isPlaying = true;
#endif

    ///Timers
    {
        Time::m_deltaTime = Time::timeScale * Time::m_unscaled_deltaTime;

        Time::m_Time += Time::m_deltaTime;
        Time::m_unscaled_Time += Time::m_unscaled_deltaTime;

        m_physics_time += Time::m_deltaTime;
    }
    ///Awake after pause
    {
        if (isPaused != EditorApplication::isPaused) {
            isPaused = EditorApplication::isPaused;

            for (auto &scene: global.scene.data) {
                if (!scene.second.isLoaded) { continue; };

                for (auto &object : scene.second.loading_awake) {
                    if (object) {
                        if (object->gameObject()->activeInHierarchy()) {
                            object->UnityAwake();

                            auto &to_awake = object->gameObject()->to_awake;
                            auto it = std::find_if(to_awake.begin(), to_awake.end(), [&object](auto &&i) {
                                return i == object;
                            });
                            if (it != to_awake.end()) { to_awake.erase(it); }
                        }
                        object = nullptr;
                    }
                }
                //TODO: Garbage collect loading_awake nullptr objects
            }
        }
    }

    ///Start
    {
        for (auto &scene: global.scene.data) {
            if (!scene.second.isLoaded) { continue; };

            for (auto &object : scene.second.new_components) {
                if (object) {
                    if (object->gameObject()->activeInHierarchy() &&
                        (Application::isPlaying() || Attributes::CheckCustomAttribute(object, ExecuteInEditMode))) {

                        object->UnityStart();

                        global.scene.components.emplace_back(object);
                        object = nullptr;
                    }
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
        if (Application::isPlaying()) {

        }

        //If my elapsed frame time is bigger than should break.
        if ((Time::m_unscaled_deltaTime + deltaClock.getElapsedTime().asSeconds()) > Time::maximumDeltaTime) {
            m_physics_time = 0;
            break;
        }
    }

    /**
     * Events
     */

    ///Update
    for (auto &object : global.scene.components) {
        auto mono = dynamic_pointer_cast<MonoBehaviour>(object);
        if (mono && mono->isActiveAndEnabled() &&
            (Application::isPlaying() || Attributes::CheckCustomAttribute(mono, ExecuteInEditMode))) {
            mono->Update(); //TODO: Call only when something happened
        }
    }


    ///Late Update
    for (auto &object : global.scene.components) {
        auto mono = dynamic_pointer_cast<MonoBehaviour>(object);
        if (mono && mono->isActiveAndEnabled() &&
            (Application::isPlaying() || Attributes::CheckCustomAttribute(object, ExecuteInEditMode))) {
            mono->LateUpdate(); //TODO: Call only when something happened
        }
    }


    ///Scene rendering
    for (auto &camera : global.m_draw_order) {
        if (camera.second && camera.second->isActiveAndEnabled()) {
            camera.second->Render();
        }
    }

    ///On Gui

    ///Wait for end of frame
}
