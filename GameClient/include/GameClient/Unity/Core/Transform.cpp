//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Transform.h"
#include <GameClient/Unity/Core/GameObject.h>

Transform::Transform() : childCount(0),
                         parent([](auto t) { /*set parent*/}, []() { return TPtr<Transform>{nullptr}; /*get*/}) {

}

TPtr<Transform> Transform::GetChild(int index) const {
    if (index >= childCount) { throw std::runtime_error("GetChild index >= childCount"); }
    return children[index]->transform();
}
