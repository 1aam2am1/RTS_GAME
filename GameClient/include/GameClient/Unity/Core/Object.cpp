//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Object.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/ObjectFactory.h>
#include <GameClient/TPtr.h>

EXPORT_CLASS(Object, ("m_Name", name));

Object::Object() {

}

Object::Object(const Object &o) : std::enable_shared_from_this<Object>(o) {
    name = o.name;
}

Object &Object::operator=(const Object &o) {
    name = o.name;
    return *this;
}

TPtr<Object> Object::Instantiate(Object *original) {
    try {
        auto[_, constructor] = MetaData::get_name_constructor(typeid(*original));
        auto result = constructor->create();

        constructor->copy(result.get(), original);

    } EXCEPTION_PRINT

    return TPtr<Object>(nullptr);
}


void Object::DestroyImmediate(Object *obj, bool allowDestroyingAssets) {
    //TODO: Thread save
    if (obj) {
        obj->onDestroySignal(nullptr);
    }
}
