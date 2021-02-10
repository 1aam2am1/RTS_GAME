//
// Created by Michal_Marszalek on 2021-01-21.
//


#include <Core/MonoBehaviour.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>
#include <GameClient/Unity/Core/Sprite.h>
#include <GameClient/Unity/Core/SpriteRenderer.h>
#include <GameClient/Unity/Physics2D/CircleCollider2D.h>
#include <random>
#include <Core/Attributes.h>
#include <Core/Camera.h>
#include <GameClient/Components/Network/NetworkInterface.h>
#include <GameClient/Components/Network/SignalSynchronizer.h>
#include "Resource.h"
#include "PrefabFunc.h"
#include "UserControls.h"
#include "AiEnemy.h"

class GenerateWorld : public MonoBehaviour {
public:
    int x = 60;
    int y = 32;
    int volume = 300;

    void Awake() override {}

    void Update() override {}

    void Start() override {
        std::random_device rd;
        std::uniform_int_distribution<> distribx(-x, x);
        std::uniform_int_distribution<> distriby(-y, y);
        std::uniform_int_distribution<> distribt(0, 3000);

        auto sprite = dynamic_pointer_cast<Sprite>(AssetDatabase::LoadAssetAtPath("Assets/32x32.png", typeid(Sprite)));
        auto network = FindObjectOfType<NetworkInterface>();

        if (!network || network->isServer()) {

            for (int i = 0; i < volume; ++i) {
                auto go = newGameObject("Asteroid " + GameApi::to_string(i));
                go->transform()->SetParent(transform(), true);

                sf::Vector3f p;
                do {
                    p.x = distribx(rd) & ~1;
                    p.y = distriby(rd) & ~1;
                } while ((abs(p.x) >= 34 && abs(p.x) <= 36) || (abs(p.y) >= 34 && abs(p.y) <= 36));

                go->transform()->localPosition = p;
                go->transform()->localScale = sf::Vector3f{0.55f, 0.55f, 1.f};

                go->AddComponent<SpriteRenderer>()->sprite = sprite;
                go->AddComponent<CircleCollider2D>()->radius = 0.3;
                auto r = go->AddComponent<Resource>();
                r->type = static_cast<ResourceType>(distribt(rd) % 3);
                r->volume = distribt(rd) + 300;

                if (network) {
                    go->AddComponent<SignalSynchronizer>()->send = true;
                }
            }

        }

        {
            auto vec = FindObjectsOfType<Enemy>();
            if (vec.size() != 2) {
                throw std::runtime_error("We need to bases.");
            }
            if (vec[0]->gameObject()->name != "UserBase") {
                std::swap(vec[0], vec[1]);
            }

            if (network && !network->isServer()) {
                std::swap(vec[0], vec[1]);
            }
            vec[0]->gameObject()->AddComponent<UserControls>();
            Camera::main()->transform()->localPosition = vec[0]->transform()->localPosition;

            if (!network)
                vec[1]->gameObject()->AddComponent<AiEnemy>();
        }

    }
};

ADD_USER_COMPONENT(GenerateWorld, x, y, volume)
//ADD_ATTRIBUTE(GenerateWorld, ExecuteInEditMode)
