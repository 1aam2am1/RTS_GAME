//
// Created by Michal_Marszalek on 2021-02-10.
//

#include "GameObjectDatabase.h"
#include "GlobalStaticVariables.h"

//TODO: Fix this as this is time boomb and is wrong
bool GameObjectDatabase::TryGetGUIDAndLocalFileIdentifier(const TPtr<const Object> &obj, Unity::GUID &guid,
                                                          Unity::fileID &localId) {

    auto go = dynamic_cast<const GameObject *>(obj.get());
    auto com = dynamic_cast<const Component *>(obj.get());

    if (com) { go = com->gameObject().get(); }

    if (go) {

        auto id = go->scene.get()->id;

        auto scene = global.scene.data.find(id);
        if (scene != global.scene.data.end()) {

            guid = scene->second.guid;
            localId = scene->second.GetID(const_cast<Object *>(obj.get())->shared_from_this());

            return true;
        }
    }

    return false;
}

TPtr<> GameObjectDatabase::TRYGetObjectFROMGUIDAndLocalFileIdentifier(Unity::GUID &guid, Unity::fileID &localId) {
    for (auto &it : global.scene.data) {
        if (it.second.guid == guid) {
            return it.second.GetObject(localId);
        }
    }
    return TPtr<>();
}

bool GameObjectDatabase::Register(TPtr<const Object> obj, Unity::fileID localId) {
    auto go = dynamic_cast<const GameObject *>(obj.get());
    auto com = dynamic_cast<const Component *>(obj.get());

    if (com) { go = com->gameObject().get(); }

    if (go) {

        auto id = go->scene.get()->id;

        auto scene = global.scene.data.find(id);
        if (scene != global.scene.data.end()) {

            return scene->second.RegisterID(localId, const_cast<Object *>(obj.get())->shared_from_this());
        }
    }

    return false;
}
