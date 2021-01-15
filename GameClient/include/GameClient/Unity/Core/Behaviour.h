//
// Created by Michal_Marszalek on 15.09.2020.
//

#ifndef RTS_GAME_BEHAVIOUR_H
#define RTS_GAME_BEHAVIOUR_H


#include "Component.h"

/// Behaviours are Components that can be enabled or disabled.
class Behaviour : public Component {
public:
    Behaviour();

    /// Enabled Behaviours are Updated, disabled Behaviours are not.
    bool enabled;

    /// Has the Behaviour had active and enabled called?
    /// \note A GameObject can be active or not active. Similarly, a script can be enabled or disabled.
    /// If a GameObject is active and has an enabled script then isActiveAndEnabled will return true.
    /// Otherwise false is returned.
    [[nodiscard]] bool isActiveAndEnabled() const;

protected:
    /// This function is called when the object becomes enabled and active.
    virtual void OnEnable() = 0;

    /// This function is called when the behaviour becomes disabled.
    virtual void OnDisable() = 0;
};


#endif //RTS_GAME_BEHAVIOUR_H
