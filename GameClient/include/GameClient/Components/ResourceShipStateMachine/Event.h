//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_EVENT_H
#define RTS_GAME_EVENT_H


#include <utility>
#include <GameClient/TPtr.h>

class ResourceShip;

struct Event {

};

struct AttackedEvent : Event {
};

struct ChangedBaseModeEvent : Event {
};

struct UpdateEvent : Event {
};

struct FixedUpdateEvent : Event {
};

#endif //RTS_GAME_EVENT_H
