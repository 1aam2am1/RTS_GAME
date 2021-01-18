//
// Created by Michal_Marszalek on 09.12.2020.
//

#include "GameLoop.h"
#include "GlobalStaticVariables.h"
#include <GameClient/GlobalStaticVariables.h>
#include <GameClient/Unity/Editor/EditorApplication.h>
#include <Core/Time.h>
#include <SceneManagement/EditorSceneManager.h>
#include <Core/MonoBehaviour.h>
#include <Core/Attributes.h>
#include <Editor/AssetDatabase.h>
#include <GameClient/Unity/Core/Transform.h>
#include <box2d/box2d.h>
#include <numbers>

#if UNITY_EDITOR
extern bool Application_isPlaying;
#endif

void GameLoop::run() {
    Time::m_unscaled_deltaTime = deltaClock.restart().asSeconds();

    static const auto synchronize_transform_to_box2d = []() {
        decltype(global.physics.transform_dirty) copy;
        std::swap(copy, global.physics.transform_dirty);

        for (auto &&t : copy) {
            if (t) {
                if (t->m_physics_root) { //RigidBody2d
                    b2Vec2 pos = {t->localPosition().x, t->localPosition().y};
                    float angle = t->localRotation * std::numbers::pi / 180.f;
                    if (t->m_physics_root->GetPosition() != pos || t->m_physics_root->GetAngle() != angle) {
                        t->m_physics_root->SetTransform(pos, angle);
                        t->m_physics_root->SetAwake(true);
                    }
                } else { //Colliders
                    for (auto &&c: t->gameObject()->GetComponents<Collider2D>()) {
                        c->Refresh(); //TODO: Make it faster
                    }
                }
                t->m_dirty_registered = false;
            }
        }
        copy.clear();
        assert(global.physics.transform_dirty.empty());
        std::swap(copy, global.physics.transform_dirty); //Declared memory back
    };

    static const auto synchronize_transform_from_box2d = []() {
        global.physics.transform_lock = true;
        auto list = global.physics.world.GetBodyList();
        while (list) {
            auto t = dynamic_pointer_cast<Transform>(list->GetUserData().lock());
            if (t) {
                t->localPosition = sf::Vector3f{list->GetPosition().x, list->GetPosition().y,
                                                t->localPosition().z};
                t->localRotation = list->GetAngle() * 180.0f / std::numbers::pi;
            }

            list = list->GetNext();
        }
        global.physics.transform_lock = false;
        assert(global.physics.transform_dirty.empty());
    };

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
                AssetDatabase::DeleteAsset("Assets/_U.unity");
                //fs::remove("Assets/_U.unity"); //Clear after yourself
                //fs::remove("Assets/_U.unity.meta");
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

    ///Start
    {
        for (auto &scene: global.scene.data) {
            if (!scene.second.isLoaded) [[unlikely]] { continue; };

            for (auto &object : scene.second.new_components) {
                if (object) [[likely]] {
                    if (object->gameObject()->activeInHierarchy() &&
                        (Application::isPlaying() || Attributes::CheckCustomAttribute(object, ExecuteInEditMode))) {

                        object->UnityStart();

                        global.scene.components.emplace_back(object);
                        object = nullptr;
                    }
                }
            }
            std::erase_if(scene.second.new_components, [](auto ob) { return ob.expired(); });
            //TODO: Garbage collect new_components nullptr objects
        }
    }

    /// Physics
    while (true) {
        if (m_physics_time <= Time::fixedDeltaTime) { break; } //should run

        Time::m_fixedTime += Time::fixedDeltaTime;
        if (Time::timeScale != 0.0f) { Time::m_unscaled_fixedTime += Time::fixedDeltaTime / Time::timeScale; }

        m_physics_time -= Time::fixedDeltaTime;

        if (Application::isPlaying()) {
            global.physics.world.ClearForces();

            for (auto &object : global.scene.components) {
                auto mono = dynamic_pointer_cast<MonoBehaviour>(object);
                if (mono && mono->isActiveAndEnabled()) {
                    mono->FixedUpdate();
                }
            }

            synchronize_transform_to_box2d();

            global.physics.world.Step(Time::fixedDeltaTime, 10, 8);

            synchronize_transform_from_box2d();

            global.physics.listener.Call(); //=> On Enter, Update velocity, Stay, On Exit
            global.physics.listener.Update(); //=> To_Stay => Stay
        } else {
            synchronize_transform_to_box2d(); //gizmo
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
            mono->Update(); //TODO: Call only when something happened in editor mode
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
    for (auto &camera : global.rendering.m_draw_order) {
        if (camera.second && camera.second->isActiveAndEnabled()) {
            camera.second->Render();
        }
    }

    ///On Gui

    ///Wait for end of frame
}
