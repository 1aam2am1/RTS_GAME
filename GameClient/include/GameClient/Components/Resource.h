//
// Created by Michal_Marszalek on 2021-01-21.
//

#ifndef RTS_GAME_RESOURCE_H
#define RTS_GAME_RESOURCE_H

#include <Core/MonoBehaviour.h>

enum class ResourceType {
    water = 0,
    metal = 1,
    food = 2
};

class Resource : public MonoBehaviour {
public:
    ResourceType type = ResourceType::water;

    int volume = 0;

    void Awake() override {}

    void Update() override {}

    void Start() override {}
};

NLOHMANN_JSON_SERIALIZE_ENUM(ResourceType,
                             {{ ResourceType::water, "water" }, { ResourceType::metal, "metal" }, { ResourceType::food, "food" }})

#endif //RTS_GAME_RESOURCE_H
