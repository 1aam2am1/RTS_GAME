//
// Created by Michal_Marszalek on 13.01.2021.
//

#include "Attributes.h"
#include <GameClient/MetaData.h>

decltype(Object::flags) Attributes::GetAttribute(const Object *o) {
    auto reflection = MetaData::getReflection(typeid(*o));
    return &reflection.flags;
}
