//
// Created by Michal_Marszalek on 23.09.2020.
//

#ifndef RTS_GAME_MACRO_H
#define RTS_GAME_MACRO_H

#define CAT(a, b) CAT2(a,b) // force expand
#define CAT2(a, b) a##b // actually concatenate
#define UNIQUE_ID(PRE) CAT(PRE,__COUNTER__)

#define TPTR_P(NAME) TPtr<Object> NAME{this}
#define TPTR_PT(TYPE, NAME) TPtr<TYPE> NAME{this}

/// Export class to save it in scene and use in in gameobject as component
#define EXPORT_CLASS(TYPE)                          \
namespace {                                         \
    int INTERNAL_NO_USE_CLASS_##TYPE = [](){        \
        ObjectFactory::register_class<TYPE>(#TYPE); \
        return 0;                                   \
    }();                                            \
}

/// Export class to save it in scene and allows you to place a
/// script anywhere in the "Component" menu, instead of just the "Component->Scripts" menu.
/// \param TYPE class type
/// \param PATH PATH and name of where to place component
/// \param ... priority
#define ADD_COMPONENT_MENU(TYPE, PATH, ...)


/// Mark a ScriptableObject-derived type to be automatically listed in the Assets/Create submenu,
/// so that instances of the type can be easily created and stored in the project as ".asset" files.
#define CREATE_ASSET_MENU(TYPE)

/// The MenuItem attribute allows you to add menu items to the main menu and inspector context menus.
/// \param FUNC void() or void(MenuCommand) or (validation) bool() or bool(MenuCommand
/// \param PATH -> PATH OR CONTEXT/CLASS_NAME/FUNC NAME
/// \param ... validation, priority
#define MENU_ITEM(FUNC, PATH, ...) \
namespace {                        \
    static int UNIQUE_ID(INTERNAL_NO_USE_F_) = [](){ \
        Menu::addItem(PATH, FUNC, {__VA_ARGS__}); \
        return 0;                 \
    }();                          \
}

/// The ContextMenu attribute allows you to add commands to the context menu of this class.
/// \param TYPE class type
/// \param FUNC void() or void(MenuCommand) or (validation) bool() or bool(MenuCommand
/// \param ... validation, priority
#define CONTEXT_MENU(TYPE, FUNC, ...)

/// The MenuItem attribute allows you to add tabs to menu
/// \param ... priority
#define MENU_TAB(PATH, ...) \
namespace {                 \
    static const int UNIQUE_ID(INTERNAL_NO_USE_T_) = [](){ \
        Menu::addPlaceHolder(PATH, {__VA_ARGS__});   \
        return 0;                 \
    }();                          \
}

/// Tells an Editor class which run-time type it's an editor for.
/// \param isFallback	If true, match this editor only if all non-fallback editors do not match. Defaults to false.
#define CUSTOM_EDITOR(TYPE, ...) //__VA_ARGS__

#endif //RTS_GAME_MACRO_H
