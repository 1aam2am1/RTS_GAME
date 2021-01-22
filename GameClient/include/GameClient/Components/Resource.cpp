//
// Created by Michal_Marszalek on 2021-01-21.
//

#include "Resource.h"
#include <Macro.h>

decltype(Resource::resources) Resource::resources;

EXPORT_ENUM(ResourceType,
            {{ ResourceType::water, "water" }, { ResourceType::metal, "metal" }, { ResourceType::food, "food" }})
EXPORT_CLASS(Resource, type, volume)
