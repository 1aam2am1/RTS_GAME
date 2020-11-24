//
// Created by Michal_Marszalek on 08.09.2020.
//

#include "SceneManager.h"
#include <filesystem>
#include <GameApi/BasicString.h>

namespace fs = std::filesystem;

uint64_t SceneManager::id = 1;
decltype(SceneManager::data) SceneManager::data{{0, {0, false}}};

void SceneManager::MoveGameObjectToScene(TPtr<GameObject> go, SceneManager::SceneP scene) {
    if (!go || !scene || !scene->isValid()) {
        throw std::runtime_error("Scene and game object should exists");
    }
    auto existing_id = go->scene.get()->id;
    auto goto_id = scene->id;

    auto it = data.find(existing_id);
    auto goto_it = data.find(goto_id);

    if (it != data.end() && goto_it != data.end()) {

        //if is root
        auto data_it = std::find_if(it->second.objects.begin(), it->second.objects.end(), [&](auto i) {
            return i == go;
        });

        if (data_it != it->second.objects.end()) {
            goto_it->second.objects.emplace_back(go);
            go->m_scene = std::shared_ptr<Scene>(new Scene(goto_id));
        }
    }
}

void SceneManager::LoadScene(std::string_view sceneName, SceneManager::LoadSceneMode mode) {
    auto meta = fs::directory_entry(sceneName);
    //TODO: Check if only filename and load path from data
    if (!meta.is_regular_file()) {
        GameApi::log(ERR.fmt("Scene %s don't exists", sceneName.data()));
        return;
    }

    auto file = GameApi::readFullFile(sceneName);

    //TODO: 1 Scene deserialization
}

