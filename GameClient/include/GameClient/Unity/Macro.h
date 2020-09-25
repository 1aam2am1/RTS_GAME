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

/// Mark a ScriptableObject-derived type to be automatically listed in the Assets/Create submenu,
/// so that instances of the type can be easily created and stored in the project as ".asset" files.
#define CreateAssetMenuAttribute(TYPE)

/// The MenuItem attribute allows you to add menu items to the main menu and inspector context menus.
#define MENU_ITEM(FUNC, PATH, ...) \
namespace {                        \
    static int UNIQUE_ID(INTERNAL_NO_USE_F_) = [](){ \
        Menu::addItem(PATH, &FUNC, {__VA_ARGS__}); \
        return 0;                 \
    }();                          \
}

#define MENU_TAB(PATH, ...) \
namespace {                 \
    static const int UNIQUE_ID(INTERNAL_NO_USE_T_) = [](){ \
        Menu::addPlaceHolder(PATH, {__VA_ARGS__});   \
        return 0;                 \
    }();                          \
}

/// Tells an Editor class which run-time type it's an editor for.
/// \param isFallback	If true, match this editor only if all non-fallback editors do not match. Defaults to false.
#define CustomEditor(TYPE, ...) //__VA_ARGS__

#endif //RTS_GAME_MACRO_H
