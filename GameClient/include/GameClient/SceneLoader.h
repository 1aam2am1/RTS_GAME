//
// Created by Michal_Marszalek on 2021-01-21.
//

#ifndef RTS_GAME_SCENELOADER_H
#define RTS_GAME_SCENELOADER_H

#include <string_view>

class SceneLoader {
public:
/// \param destroy_on_load Destroy gameobjects marked as don't destroy on load?
    static void LoadSceneFull(uint32_t id, std::string_view scenePath, bool single = true, bool destroy_on_load = true);
};


#endif //RTS_GAME_SCENELOADER_H
