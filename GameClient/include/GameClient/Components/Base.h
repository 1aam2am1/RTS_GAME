//
// Created by Michal_Marszalek on 2021-01-22.
//

#ifndef RTS_GAME_BASE_H
#define RTS_GAME_BASE_H


#include <Core/MonoBehaviour.h>
#include "Resource.h"

class Base : public MonoBehaviour {
public:
    std::map<ResourceType, float> resources = {{ResourceType::food,  100},
                                               {ResourceType::metal, 200},
                                               {ResourceType::water, 100}};
};


#endif //RTS_GAME_BASE_H
