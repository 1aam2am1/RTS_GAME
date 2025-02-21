//
// Created by Michal_Marszalek on 2021-01-24.
//

#include <Core/SpriteRenderer.h>
#include <Physics2D/PolygonCollider2D.h>
#include <Editor/AssetDatabase.h>
#include <Physics2D/BoxCollider2D.h>
#include <GameClient/Components/Life/Life.h>
#include <Physics2D/CircleCollider2D.h>
#include <GameClient/Components/Life/Attack.h>
#include <GameClient/Components/Network/FullSynchronizer.h>
#include <GameClient/Components/Network/NetworkInterface.h>
#include "PrefabFunc.h"
#include "Enemy.h"
#include "ResourceShip.h"
#include "AttackShip.h"
#include "Building.h"
#include "ResourcePoint.h"
#include "Bullet.h"

TPtr<GameObject> Prefab_func::create_ship(TPtr<Enemy> parent, ShipType type, sf::Vector3f position) {
    auto go = newGameObject("Ship");
    go->AddComponent<PolygonCollider2D>();
    go->AddComponent<Rigidbody2D>();
    go->AddComponent<Life>();
    if (NetworkInterface::network())
        go->AddComponent<FullSynchronizer>()->send = true;

    auto sr = go->AddComponent<SpriteRenderer>();
    switch (type) {
        case ShipType::Attack: {
            sr->sprite = parent->attack_sprite;
            auto r = go->AddComponent<AttackShip>();
            r->parent = parent;
            r->attack_force = 7;
        }
            break;
        case ShipType::Resource: {
            sr->sprite = parent->resource_sprite;
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
    go->layer = parent->gameObject()->layer;
    if (NetworkInterface::network())
        go->AddComponent<FullSynchronizer>()->send = true;

    auto sr = go->AddComponent<SpriteRenderer>();
    go->AddComponent<Life>()->life = 30;
    switch (type) {
        case ShipType::Resource:
            sr->sprite = dynamic_pointer_cast<Sprite>(
                    AssetDatabase::LoadAssetAtPath("Assets/zasoby_zbieranie.png", typeid(Sprite)));
            go->AddComponent<ResourcePoint>();
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

TPtr<GameObject> Prefab_func::create_bullet(TPtr<GameObject> parent, sf::Vector3f position, float direction) {
    auto go = newGameObject("Bullet");
    go->layer = parent->layer;
    if (NetworkInterface::network())
        go->AddComponent<FullSynchronizer>()->send = true;

    go->transform()->localPosition = position;
    go->transform()->localRotation = direction;

    auto rig = go->AddComponent<Rigidbody2D>();
    rig->inertia = 0;

    auto sr = go->AddComponent<SpriteRenderer>();
    sr->sprite = dynamic_pointer_cast<Sprite>(AssetDatabase::LoadAssetAtPath("Assets/pixel-16x16.png", typeid(Sprite)));

    auto box = go->AddComponent<CircleCollider2D>();
    box->radius = 0.2f;

    go->AddComponent<Attack>();
    go->AddComponent<Bullet>();

    return go;
}
