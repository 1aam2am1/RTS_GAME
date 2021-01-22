//
// Created by Michal_Marszalek on 2021-01-22.
//

#include "mono_state.h"
#include <Macro.h>


EXPORT_ENUM(mono_state,
            {
                { mono_state::wait, "wait" }, { mono_state::attack, "attack" }, { mono_state::flee, "flee" },
                { mono_state::resource, "resource" }
            })
