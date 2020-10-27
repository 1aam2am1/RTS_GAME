//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Object.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/ObjectFactory.h>

EXPORT_CLASS(Object, name);

Object::Object() {

}

Object::Object(const Object &o) : std::enable_shared_from_this<Object>(o) {
    name = o.name;
}

Object &Object::operator=(const Object &o) {
    name = o.name;
    return *this;
}
