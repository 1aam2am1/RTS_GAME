//
// Created by Michal_Marszalek on 2021-01-22.
//

#ifndef RTS_GAME_AIENEMY_H
#define RTS_GAME_AIENEMY_H

#include "Enemy.h"
#include <GameClient/Unity/Core/SpriteRenderer.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>

class AiEnemy : public MonoBehaviour {
public:
    TPtr<Enemy> base;
    float time_to_decision = 0.f;
    uint32_t depth = 20;

    ~AiEnemy() {
        if (t.joinable()) { t.detach(); }
    }

    void Start() override {
        base = GetComponent<Enemy>();

        auto vec = AssetDatabase::LoadAllAssetRepresentationsAtPath("Assets/ships/BlueRedGreen_Spacecraft_V1.0.png");

        for (auto &&v : vec) {
            Unity::GUID guid;
            Unity::fileID id;

            AssetDatabase::TryGetGUIDAndLocalFileIdentifier(v, guid, id);

            if (id == 15024732907312285943llu) {
                base->ship_sprites[(int) ShipType::Attack] = dynamic_pointer_cast<Sprite>(v);
            }

            if (id == 15024732907312285940llu) {
                base->ship_sprites[(int) ShipType::Resource] = dynamic_pointer_cast<Sprite>(v);
            }
        }
    }

    void Update() override;

private:
    enum Actions {
        Attack,
        Wait,
        Build_attack_ship,
        Build_resource_ship,
        Build_attack_building,
        Build_resource_building
    };

    std::thread t;
    std::atomic<bool> thread_runs = false;

    struct Data;

    static float min_max(const Data &d, int depth, bool max, float tA, float tB, float alfa, float beta);

    static float calculate(const Data &);

    static std::pair<std::array<Actions, 6>, size_t> actions(const Data &, bool max);

    static Data simulate(Data d, Actions action, bool max, float time);

    static float NextActionTime(const Data &d, bool max);
};


#endif //RTS_GAME_AIENEMY_H
