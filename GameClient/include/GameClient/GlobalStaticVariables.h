//
// Created by Michal_Marszalek on 15.12.2020.
//

#ifndef RTS_GAME_GLOBALSTATICVARIABLES_H
#define RTS_GAME_GLOBALSTATICVARIABLES_H

#include <vector>
#include <map>
#include <GameClient/Unity/Core/Camera.h>
#include <SFML/Graphics/RenderTexture.hpp>

struct GlobalStaticVariables {
    ///Camera
    std::vector<Camera *> m_cameras{};
    std::multimap<float, Camera *> m_draw_order{};
#if UNITY_EDITOR
    sf::RenderTexture m_target;
#else
    sf::RenderTarget m_target;
#endif
};

extern GlobalStaticVariables global;


#endif //RTS_GAME_GLOBALSTATICVARIABLES_H
