//
// Created by Michal_Marszalek on 2021-01-21.
//

#ifndef RTS_GAME_RESOURCE_H
#define RTS_GAME_RESOURCE_H

#include <Core/MonoBehaviour.h>
#include <GameClient/Components/Network/SignalSynchronizer.h>

enum class ResourceType {
    water = 0,
    metal = 1,
    food = 2
};

class Resource : public MonoBehaviour {
public:
    ResourceType type = ResourceType::water;

    float GetVolume() const { return volume; }

    void RemoveFromVolume(float v) {
        volume -= v;
        if (signal) {
            signal->SendMessage(0, {{"v", v}});
        }
    }

    void Awake() override {}

    void Update() override {
        if (volume == 0) {
            Destroy(gameObject());
        }
    }

    void Start() override {
        resources.emplace_back(shared_from_this());
        signal = GetComponent<SignalSynchronizer>();
        if (signal) {
            signal->OnMessage.connect(&Resource::OnGet, this);
        }
    }

    void OnDestroy() override {
        auto it = std::find_if(resources.begin(), resources.end(), [this](auto &&r) {
            return r.get() == this;
        });
        if (it != resources.end()) {
            resources.erase(it);
        }
    }


    static std::vector<TPtr<Resource>> resources;

private:
    friend class GenerateWorld;

    TPtr<SignalSynchronizer> signal;

    float volume = 0;

    void OnGet(int i, nlohmann::json j);
};

NLOHMANN_JSON_SERIALIZE_ENUM(ResourceType,
                             {{ ResourceType::water, "water" }, { ResourceType::metal, "metal" }, { ResourceType::food, "food" }})

#endif //RTS_GAME_RESOURCE_H
