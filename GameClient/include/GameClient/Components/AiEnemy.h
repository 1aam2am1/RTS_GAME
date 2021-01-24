//
// Created by Michal_Marszalek on 2021-01-22.
//

#ifndef RTS_GAME_AIENEMY_H
#define RTS_GAME_AIENEMY_H

#include "Enemy.h"
#include <GameClient/Unity/Core/SpriteRenderer.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>

class AiEnemy : public Enemy {
public:
    void Start() override {
        Enemy::Start();

        auto vec = AssetDatabase::LoadAllAssetRepresentationsAtPath("Assets/ships/BlueRedGreen_Spacecraft_V1.0.png");

        for (auto &&v : vec) {
            Unity::GUID guid;
            Unity::fileID id;

            AssetDatabase::TryGetGUIDAndLocalFileIdentifier(v, guid, id);

            if (id == 15024732907312285943llu) {
                ship_sprites[(int) ShipType::Attack] = dynamic_pointer_cast<Sprite>(v);
            }

            if (id == 15024732907312285940llu) {
                ship_sprites[(int) ShipType::Resource] = dynamic_pointer_cast<Sprite>(v);
            }
        }
    }

    void Update() override;
};


#endif //RTS_GAME_AIENEMY_H
