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

class GenerateWorld : public MonoBehaviour {
public:
    void Awake() override {}

    void Update() override {}

    void Start() override {
        std::random_device rd;
        std::uniform_int_distribution<> distrib(-100, 100);

        auto sprite = dynamic_pointer_cast<Sprite>(AssetDatabase::LoadAssetAtPath("Assets/32x32.png", typeid(Sprite)));

        for (uint32_t i = 0; i < 600; ++i) {
            auto go = newGameObject("Asteroid " + GameApi::to_string(i));
            go->AddComponent<SpriteRenderer>()->sprite = sprite;
            go->AddComponent<CircleCollider2D>()->radius = 0.3;

            go->transform()->SetParent(transform(), true);

            sf::Vector3f p;
            p.x = distrib(rd);
            p.y = distrib(rd);
            go->transform()->localPosition = p;
            go->transform()->localScale = sf::Vector3f{0.55f, 0.55f, 1.f};
        }

    }
};

ADD_USER_COMPONENT(GenerateWorld)
