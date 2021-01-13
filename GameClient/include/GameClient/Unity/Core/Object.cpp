//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Object.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Core/Transform.h>
#include <GameClient/MainThread.h>

EXPORT_CLASS(Object, ("m_Name", name));

TPtr<Object> Object::shared_from_this() {
    return TPtr<Object>{std::enable_shared_from_this<Object>::shared_from_this()};
}

TPtr<Object const> Object::shared_from_this() const {
    return TPtr<Object const>{std::enable_shared_from_this<Object>::shared_from_this()};
}

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

        return result;
    } EXCEPTION_PRINT

    //TODO: Add to scene if game object, call Awake and so on...
    return {};
}


void Object::DestroyImmediate(TPtr<Object> obj, bool allowDestroyingAssets) {
    //TODO: Thread save
    if (obj) {
        if (auto t = dynamic_cast<Transform *>(obj.get())) {
            if (auto g = t->gameObject()) {
                DestroyImmediate(g, allowDestroyingAssets);
                return;
            }
        }

        std::shared_ptr<Object> copy = static_cast<std::enable_shared_from_this<Object> *>(obj.get())->shared_from_this();
        std::weak_ptr<Object> memory_release = copy;

        //TODO: OnDestroy
        obj->onDestroySignal(nullptr);

        MainThread::Invoke([memory_release]() {});

    }
}
