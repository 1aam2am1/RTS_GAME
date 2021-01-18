//
// Created by Michal_Marszalek on 2021-01-18.
//

#ifndef RTS_GAME_CONTACTLISTENER_H
#define RTS_GAME_CONTACTLISTENER_H

#include <box2d/box2d.h>
#include <unordered_map>
#include <unordered_set>
#include <GameClient/Unity/Physics2D/Collision2D.h>
#include <GameClient/Unity/Physics2D/Collider2D.h>

class ContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact *contact) override;

    void EndContact(b2Contact *contact) override;

    void Update();

    void Call();

    bool empty();

private:
    std::vector<std::pair<Collision2D, Collision2D>> m_new;
    std::unordered_set<b2Contact *> to_stay;
    std::unordered_map<b2Contact *, std::pair<Collision2D, Collision2D>> m_stays;
    std::vector<std::pair<Collision2D, Collision2D>> m_exit;

    std::vector<std::pair<TPtr<GameObject>, TPtr<Collider2D>>> m_trigger_new;
    std::unordered_set<b2Contact *> to_trigger_stay;
    std::unordered_map<b2Contact *, std::pair<TPtr<GameObject>, TPtr<Collider2D>>> m_trigger_stays;
    std::vector<std::pair<TPtr<GameObject>, TPtr<Collider2D>>> m_trigger_exit;
};


#endif //RTS_GAME_CONTACTLISTENER_H
