//
// Created by Michal_Marszalek on 16.10.2020.
//

#ifndef RTS_GAME_TEXTURE2D_H
#define RTS_GAME_TEXTURE2D_H

#include <GameClient/Unity/Core/Object.h>
#include <SFML/Graphics/Texture.hpp>

class Texture2D : public Object {
public:
    //TODO: Delete texture before ending main function as deleting main can be after deleting render target context.
    sf::Texture t0;
};


#endif //RTS_GAME_TEXTURE2D_H
