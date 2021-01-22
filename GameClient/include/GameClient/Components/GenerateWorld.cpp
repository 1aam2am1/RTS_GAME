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
#include "Resource.h"

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
//600 100 100
        for (int i = 0; i < volume; ++i) {
            auto go = newGameObject("Asteroid " + GameApi::to_string(i));
            sf::Vector3f p;
            p.x = distribx(rd);
            p.y = distriby(rd);

            go->transform()->SetParent(transform(), true);
            go->transform()->localPosition = p;
            go->transform()->localScale = sf::Vector3f{0.55f, 0.55f, 1.f};

            go->AddComponent<SpriteRenderer>()->sprite = sprite;
            go->AddComponent<CircleCollider2D>()->radius = 0.3;
            auto r = go->AddComponent<Resource>();
            r->type = static_cast<ResourceType>(distribt(rd) % 3);
            r->volume = distribt(rd) + 300;
        }
    }
};

ADD_USER_COMPONENT(GenerateWorld, x, y, volume)
