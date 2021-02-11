//
// Created by Michal_Marszalek on 2021-02-02.
//

#include "Life.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/GameObjectDatabase.h>

ADD_USER_COMPONENT(Life, life)

void Life::attacked(float i, const TPtr<GameObject> &enemy) {
    GUIDFileIDPack pack;
    if (signal && GameObjectDatabase::TryGetGUIDAndLocalFileIdentifier(enemy, pack.guid, pack.id)) {
        signal->SendMessage(1, {{"pack",   pack},
                                {"attack", i}});

        auto it = std::find_if(attacker.begin(), attacker.end(), [&](auto &g) { return g == pack; });
        if (it == attacker.end()) {
            attacker.push_back(pack);
            life -= i;
        }
    } else {
        life -= i;
    }

    if (life <= 0) {
        Destroy(gameObject());
    }
}

void Life::Start() {
    signal = GetComponent<Synchronizer>();

    signal->OnMessage.connect([&](uint32_t id, auto json) {
        if (!json.contains("pack") || !json.contains("attack") || id != 1) { return; }
        GUIDFileIDPack pack;
        json.at("pack").get_to(pack);
        auto it = std::find_if(attacker.begin(), attacker.end(), [&](auto &g) { return g == pack; });
        if (it == attacker.end()) {
            attacker.push_back(pack);
            life -= json.at("attack").template get<float>();

            if (life <= 0) {
                Destroy(gameObject());
            }
        }
    });
}
