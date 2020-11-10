//
// Created by Michal_Marszalek on 01.11.2020.
//

#include "AssetImportContext.h"
#include <GameClient/Unity/Editor/OneGuidFile.h>


void AssetImportContext::AddObjectToAsset(Unity::fileID identifier, TPtr<Object> obj, TPtr<Texture2D> thumbnail) {
    if (data && obj) {
        auto is = std::find_if(data->object.begin(), data->object.end(), [&](auto &&it) {
            return it.second == obj;
        });
        if (is != data->object.end()) {
            GameApi::log(ERR.fmt("Tried adding added object %s %llu -> %llu",
                                 GameApi::demangle(typeid(*obj.get()).name()).data(),
                                 is->first,
                                 identifier));
        }

        auto[it, b] = data->object.emplace(identifier, obj);
        if (!data->main) { data->main = obj; }
        if (!b) {
            GameApi::log(ERR.fmt("Tried adding object %s with the same identifier %llu",
                                 GameApi::demangle(typeid(*obj.get()).name()).data(),
                                 identifier));
        }
    } else {
        GameApi::log(ERR.fmt("Tried adding empty object or to empty context"));
    }
}

void AssetImportContext::DependsOnSourceAsset(std::string path) {
    if (data) {
        data->dependency.emplace(path);
    }
}

void AssetImportContext::GetObjects(std::vector<TPtr<Object>> &objects) {
    objects.clear();
    if (data) {
        objects.reserve(data->object.size());
        std::for_each(data->object.begin(), data->object.end(), [&](auto &&it) {
            objects.emplace_back(it.second);
        });
    }
}

void AssetImportContext::SetMainObject(TPtr<Object> obj) {
    if (data) {
        auto is = std::find_if(data->object.begin(), data->object.end(), [&](auto &&it) {
            return it.second == obj;
        });
        if (is == data->object.end()) {
            GameApi::log(ERR.fmt("Tried setting object as main without adding it first"));

            Unity::fileID id = 0;
            while (data->object.find(id) != data->object.end()) { ++id; }
            data->object.emplace(id, obj);
        }
        data->main = obj;
    }
}

AssetImportContext::AssetImportContext(OneGUIDFile *d) : mainObject(d->main) {
    data = d;
}
