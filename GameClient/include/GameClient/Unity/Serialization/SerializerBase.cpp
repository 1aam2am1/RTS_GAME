//
// Created by Michal_Marszalek on 28.10.2020.
//

#include "SerializerBase.h"
#include <GameClient/TPtr.h>
#include <GameClient/MetaData.h>

TPtr<Object> SerializerBase::Deserialize(std::string_view str, const nlohmann::json &j) {
    return Deserialize(MetaData::get_type(str), j);
}
