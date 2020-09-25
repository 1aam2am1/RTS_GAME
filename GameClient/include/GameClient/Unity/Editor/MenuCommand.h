//
// Created by Michal_Marszalek on 24.09.2020.
//

#ifndef RTS_GAME_MENUCOMMAND_H
#define RTS_GAME_MENUCOMMAND_H

class Object;

/// Used to extract the context for a MenuItem.
/// \details MenuCommand objects are passed to custom menu item functions defined using the MenuItem attribute.
class MenuCommand {

    /// Context is the object that is the target of a menu command.
    Object *context = nullptr;
};


#endif //RTS_GAME_MENUCOMMAND_H
