//
// Created by Michal_Marszalek on 2021-01-21.
//

#include "Resource.h"
#include <Macro.h>

decltype(Resource::resources) Resource::resources;

EXPORT_ENUM(ResourceType,
            {{ ResourceType::water, "water" }, { ResourceType::metal, "metal" }, { ResourceType::food, "food" }})
EXPORT_CLASS(Resource, type, volume)

void Resource::OnGet(int i, nlohmann::json j) {
    if (i == 0) {
        try {
            volume -= j["v"].get<float>();
        } EXCEPTION_PRINT
    }
}
