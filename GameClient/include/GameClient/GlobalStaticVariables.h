//
// Created by Michal_Marszalek on 15.12.2020.
//

#ifndef RTS_GAME_GLOBALSTATICVARIABLES_H
#define RTS_GAME_GLOBALSTATICVARIABLES_H

#include <vector>
#include <map>
#include <GameClient/Unity/Core/Camera.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <Core/Renderer.h>

struct SceneData {
    int buildIndex = 0;
    bool isLoaded = false;
    std::string name = "Scene";
    std::string path{};
    std::vector<TPtr<GameObject>> root{};
    std::vector<TPtr<Component>> components{};
    std::vector<TPtr<Component>> new_components{};
};

struct GlobalStaticVariables {
    ///Camera
    std::vector<TPtr<Camera>> m_cameras{};
    std::multimap<float, TPtr<Camera>> m_draw_order{};

    std::vector<TPtr<Renderer>> m_render{};
#if UNITY_EDITOR
    sf::RenderTexture m_target;
#else
    sf::RenderTarget m_target;
#endif

    struct {
        uint64_t max_id = 0; ///< Max used id
        std::map<uint64_t, SceneData> data{}; ///< Loaded scenes
        //std::vector<uint64_t> index_to_id{1}; ///< data_id[]
        uint64_t active_scene = 0; ///< Now active scene
    } scene;
};

extern GlobalStaticVariables global;


#endif //RTS_GAME_GLOBALSTATICVARIABLES_H
