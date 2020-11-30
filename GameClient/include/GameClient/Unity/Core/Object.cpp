//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Object.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/ObjectFactory.h>
#include <GameClient/TPtr.h>
#include <GameClient/Unity/Core/Transform.h>

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
        auto reflection = MetaData::getReflection(typeid(*original));
        auto result = reflection.CreateInstance();

        reflection.CopyInstance(result.get(), original);

    } EXCEPTION_PRINT

    //TODO: Add to scene if game object, call Awake and so on...
    return TPtr<Object>(nullptr);
}


void Object::DestroyImmediate(Object *obj, bool allowDestroyingAssets) {
    //TODO: Thread save
    if (obj) {
        if (auto t = dynamic_cast<Transform *>(obj)) {
            if (auto g = t->gameObject()) {
                DestroyImmediate(g.get(), allowDestroyingAssets);
            }
        } else {
            obj->onDestroySignal(nullptr);
        }
    }
}
