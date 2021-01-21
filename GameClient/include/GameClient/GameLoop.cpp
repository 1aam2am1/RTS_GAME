//
// Created by Michal_Marszalek on 09.12.2020.
//

#include "GameLoop.h"
#include <GameClient/GlobalStaticVariables.h>
#include <GameClient/Unity/Editor/EditorApplication.h>
#include <GameClient/Unity/Core/Application.h>
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

            //Save old scene and save path to reload it.
            old_scene.path = global.scene.data[global.scene.active_scene].path;
            old_scene.name = global.scene.data[global.scene.active_scene].name;
            old_scene.old_id = global.scene.active_scene;
            EditorSceneManager::SaveScene(SceneManager::GetActiveScene(), "Assets/_U.unity", true);


            //What to load?
            std::string to_load;
            if (EditorSceneManager::playModeStartScene) {
                to_load = AssetDatabase::GetAssetPath(EditorSceneManager::playModeStartScene.get());
            } else {
                to_load = "Assets/_U.unity";
            }

            //Load scene
            auto scene = EditorSceneManager::OpenScene(to_load);
            if (!scene->isValid()) {
                EditorApplication::isPlaying = false;
                isPlaying = false;
            } else {
                if (global.scene.data[global.scene.active_scene].name == "_U") {
                    global.scene.data[global.scene.active_scene].name = old_scene.name;
                }
            }

            global.settings.Apply();
        } else {
            //Load old scene
            if (EditorSceneManager::OpenScene("Assets/_U.unity")) {
                global.scene.data[global.scene.active_scene].path = old_scene.path;
                global.scene.data[global.scene.active_scene].name = old_scene.name;

                auto new_s = global.scene.data.extract(global.scene.active_scene);
                new_s.key() = old_scene.old_id;
                global.scene.data.insert(std::move(new_s));
                global.scene.active_scene = old_scene.old_id;

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
            scene.second.new_components_copy.clear();
            std::swap(scene.second.new_components, scene.second.new_components_copy);

            for (auto &object : scene.second.new_components_copy) {
                if (object) [[likely]] {
                    if (object->gameObject()->activeInHierarchy() &&
                        (Application::isPlaying() || Attributes::CheckCustomAttribute(object, ExecuteInEditMode))) {

                        object->UnityStart();

                        global.scene.components.emplace_back(object);
                        object = nullptr;
                    } else {
                        scene.second.new_components.emplace_back(object);
                    }
                }
            }
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
                auto beh = dynamic_pointer_cast<Behaviour>(object);
                if (beh) {
                    beh->UnityFixedUpdate();
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
        auto beh = dynamic_pointer_cast<Behaviour>(object);
        if (beh &&
            (Application::isPlaying() || Attributes::CheckCustomAttribute(beh, ExecuteInEditMode))) {
            beh->UnityUpdate(); //TODO: Call only when something happened in editor mode
        }
    }


    ///Late Update
    for (auto &object : global.scene.components) {
        auto beh = dynamic_pointer_cast<Behaviour>(object);
        if (beh &&
            (Application::isPlaying() || Attributes::CheckCustomAttribute(beh, ExecuteInEditMode))) {
            beh->UnityLateUpdate(); //TODO: Call only when something happened
        }
    }

    ///Delete objects
    {
        if (Application::isPlaying()) {
            decltype(global.mis.before_draw) copy;
            std::swap(copy, global.mis.before_draw);
            for (auto &&c : copy) {
                c();
            }
        }
    }

    ///Scene rendering
    for (auto &camera : global.rendering.m_draw_order) {
        if (camera.second && camera.second->isActiveAndEnabled()) {
            camera.second->Render();
        }
    }
}
