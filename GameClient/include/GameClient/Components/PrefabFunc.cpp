//
// Created by Michal_Marszalek on 2021-01-24.
//

#include <Core/SpriteRenderer.h>
#include <Physics2D/PolygonCollider2D.h>
#include <Editor/AssetDatabase.h>
#include <Physics2D/BoxCollider2D.h>
#include "PrefabFunc.h"
#include "Enemy.h"
#include "ResourceShip.h"
#include "Building.h"

TPtr<GameObject> Prefab_func::create_ship(TPtr<Enemy> parent, ShipType type, sf::Vector3f position) {
    auto go = newGameObject("Ship");
    go->AddComponent<PolygonCollider2D>();
    go->AddComponent<Rigidbody2D>();
    auto sr = go->AddComponent<SpriteRenderer>();
    sr->sprite = parent->ship_sprites[(int) type];
    switch (type) {
        case ShipType::Attack: {
            auto r = go->AddComponent<ResourceShip>(); //TODO: Change here
            r->parent = parent;
        }
            break;
        case ShipType::Resource: {
            auto r = go->AddComponent<ResourceShip>();
            r->parent = parent;

        }
            break;
    }

    go->transform()->localPosition = position;

    return go;
}

TPtr<GameObject> Prefab_func::create_building(TPtr<Enemy> parent, ShipType type) {
    auto go = newGameObject("Building");
    auto sr = go->AddComponent<SpriteRenderer>();
    switch (type) {
        case ShipType::Resource:
            sr->sprite = dynamic_pointer_cast<Sprite>(
                    AssetDatabase::LoadAssetAtPath("Assets/zasoby_zbieranie.png", typeid(Sprite)));
            break;
        case ShipType::Attack:
            sr->sprite = dynamic_pointer_cast<Sprite>(
                    AssetDatabase::LoadAssetAtPath("Assets/build_ships.png", typeid(Sprite)));
            break;
    }
    go->AddComponent<BoxCollider2D>();
    auto bu = go->AddComponent<Building>();
    bu->parent = parent;
    bu->type = type;

    return go;
}
