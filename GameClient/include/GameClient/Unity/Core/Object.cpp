//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Object.h"
#include "ScriptableObject.h"
#include "Time.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Core/Transform.h>
#include <GameClient/MainThread.h>
#include <GameClient/GlobalStaticVariables.h>

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
    //TODO: lock obj so that we could not destroy it from destroy

    if (obj) {
        if (auto t = dynamic_cast<Transform *>(obj.get())) {
            if (auto g = t->gameObject()) {
                DestroyImmediate(g, allowDestroyingAssets);
                return;
            }
        }

        std::shared_ptr<Object> copy = static_cast<std::enable_shared_from_this<Object> *>(obj.get())->shared_from_this();
        std::weak_ptr<Object> memory_release = copy;

        if (auto component = dynamic_cast<Component *>(obj.get())) {
            component->UnityOnActiveChange(false);
            if (component->m_unityAwakeed) {
                component->OnDestroy();
            }
            if (auto go = component->gameObject()) {
                std::erase_if(go->components, [&](auto &&c) { return c.get() == component; });
            }
        } else if (auto scriptableOb = dynamic_cast<ScriptableObject *>(obj.get())) {
            scriptableOb->OnDestroy();
        }

        obj->onDestroySignal(nullptr);

        MainThread::Invoke([memory_release]() {});

    }
}

void Object::Destroy(TPtr<Object> obj, float t) {
    if (obj)
        global.mis.before_draw.emplace_back([obj, t]() mutable {
            t -= Time::deltaTime();
            if (t <= 0.f) { DestroyImmediate(obj, false); }
            else {
                Destroy(obj);
            }
        });
}

void Object::DontDestroyOnLoad(TPtr<Object> target) {
    auto go = dynamic_pointer_cast<GameObject>(target);
    if (go)
        global.scene.dont_destroy.emplace_back(go);
}

TPtr<Object> Object::PFindObjectOfType(const std::function<bool(TPtr<>)> &check) {
    for (auto &o : global.scene.all_comoponents) {
        if (check(o)) {
            return o;
        }
    }
    return {};
}

std::vector<TPtr<Object>> Object::PFindObjectsOfType(const std::function<bool(TPtr<>)> &check) {
    std::vector<TPtr<>> result;
    result.reserve(32);

    for (auto &o : global.scene.all_comoponents) {
        if (check(o)) {
            result.emplace_back(o);
        }
    }

    return result;
}
