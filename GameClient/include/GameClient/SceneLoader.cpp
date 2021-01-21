//
// Created by Michal_Marszalek on 2021-01-21.
//

#include "SceneLoader.h"
#include "GlobalStaticVariables.h"
#include <Serialization/SceneSerializer.h>
#include <Editor/AssetDatabase.h>
#include <GameClient/MainThread.h>
#include <GameClient/Unity/Editor/EditorApplication.h>
#include <GameClient/Unity/Core/Attributes.h>
#include <Macro.h>
#include <SceneManagement/SceneManager.h>

void SceneLoader::LoadSceneFull(uint32_t new_id, std::string_view scenePath, bool single, bool destroy) {
    global.scene.on_load_active_id[std::this_thread::get_id()] = new_id;
    std::shared_ptr<int> lock((int *) 1,
                              [](auto) { global.scene.on_load_active_id.erase(std::this_thread::get_id()); });

    try {
        auto str = GameApi::readFullFile(scenePath);
        auto json = nlohmann::json::parse(str);

        {
            SceneSerializer serializer;
            std::vector<TPtr<>> result;

            serializer.Deserialize(result, json["objects"]);
        }
        //creation of gameobjects adds them to the scene

        std::function<void(const TPtr<GameObject> &)> fix = [&fix](const TPtr<GameObject> &go) {
            if (!go) { return; }

            decltype(go->components) copy = std::move(go->components);
            go->components.clear();

            for (auto &c : copy) {
                go->AddComponent(c);
            }

            for (int i = 0; i < go->transform()->childCount(); ++i) {
                auto child = go->transform()->GetChild(i);
                fix(child);
            }
        };

        for (auto &ob : global.scene.data[new_id].root) {
            fix(ob);
        }

        if (single) {
            if (!destroy) {
                for (auto &c : global.scene.dont_destroy) {
                    if (c) {
                        c->scene = SceneManager::GetActiveScene();
                    }
                }
            }

            auto copy = global.scene.data.extract(new_id);
            auto old = std::move(global.scene.data);
            global.scene.data.clear();

            global.scene.data.insert(std::move(copy));
            global.scene.active_scene = new_id;

            MainThread::Invoke([old]() {}); //< Here delete old data, as this can be called from update,
            // by inexperienced user we save him, by deleting objects in new frame
        }


        global.scene.data[new_id].isLoaded = true;
        for (auto &ob : global.scene.data[new_id].loading_awake) {
            assert(ob->gameObject()->activeInHierarchy());
            if (EditorApplication::isPlaying || Attributes::CheckCustomAttribute(ob, ExecuteInEditMode)) {
                try {
                    ob->UnityAwake();
                } EXCEPTION_PRINT
                global.scene.data[new_id].new_components.emplace_back(ob); //To Start()
            }
        }
        global.scene.data[new_id].loading_awake.clear();
        for (auto &ob : global.scene.data[new_id].new_components) {
            try {
                ob->UnityOnActiveChange(true); //OnEnable()
            } EXCEPTION_PRINT
        }

    } catch (const std::exception &e) {
        global.scene.data.erase(new_id);
        GameApi::log(ERR.fmt("%s", e.what()));
    }
}
