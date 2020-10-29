//
// Created by Michal_Marszalek on 23.09.2020.
//

#ifndef RTS_GAME_MACRO_H
#define RTS_GAME_MACRO_H

#include <GameClient/Unity/forEachMacro.h>

//region UNPAREN
#define NOTHING_EXTRACT
#define EXTRACT(...) EXTRACT __VA_ARGS__
#define UNPAREN(x) CONCATENATE(NOTHING_, EXTRACT x)

#define RMFS_SERIALIZE_1(type, arg) STRINGIZE(arg), &type::arg
#define RMFS_SERIALIZE_2(type, name, arg) name, &type::arg
#define RMFS_SERIALIZE_3(type, name, set, get) name, &type::set, &type::get

#define RMFS_SERIALIZE_(N, type, ...) CONCATENATE(RMFS_SERIALIZE_, N)(type, __VA_ARGS__)
#define RMFS_SERIALIZE(type, ...) RMFS_SERIALIZE_(FOR_EACH_NARG(__VA_ARGS__), type, __VA_ARGS__)

//TODO: Change ____VA_ARGS__ and REGISTER_MEMBER_FOR_SERIALIZE macro so that you can place (x) or ("x", x) or ("x", set, get)
#define REGISTER_MEMBER_FOR_SERIALIZE(arg, TYPE) registerMemberForSerialize(RMFS_SERIALIZE(TYPE, UNPAREN(arg)))
//endregion

#define UNIQUE_ID(PRE) CONCATENATE(PRE, __COUNTER__)

#define TPTR_P(NAME) TPtr<Object> NAME{this}
#define TPTR_PT(TYPE, NAME) TPtr<TYPE> NAME{this}

/// TODO: Change registering each parameter to registering two functions -> serialize, deserialize, and if object then callback serialization???
/// Export class to save it in scene and use in in gameobject as component
/// What should be serialized
#define EXPORT_CLASS(TYPE, ...)                     \
namespace {                                         \
    static int INTERNAL_NO_USE_CLASS_##TYPE = Initializer::add([](){ \
        auto& t = ObjectFactory::register_class<TYPE>(#TYPE); \
        FOR_EACH(t.REGISTER_MEMBER_FOR_SERIALIZE, TYPE, __VA_ARGS__) \
        return 0;                                   \
    });                                            \
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
    static int UNIQUE_ID(INTERNAL_NO_USE_F_) = Initializer::add([](){ \
        Menu::addItem(PATH, FUNC, {__VA_ARGS__}); \
        return 0;                 \
    });                          \
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
    static const int UNIQUE_ID(INTERNAL_NO_USE_T_) = Initializer::add([](){ \
        Menu::addPlaceHolder(PATH, {__VA_ARGS__});   \
        return 0;                 \
    });                          \
}

/// Tells an Editor class which run-time type it's an editor for.
/// \param isFallback	If true, match this editor only if all non-fallback editors do not match. Defaults to false.
#define CUSTOM_EDITOR(TYPE, ...) //__VA_ARGS__


/// Define break in pointer that could break in inside the private class
#define DEFINE_BREAK_IN(name, t, member)\
           struct name {\
             using type = t;\
             friend constexpr t break_in(name);\
           };\
           template struct break_in_<name, member>

//DEFINE_BREAK_IN(nazwa_odwolamnia, int Klasa::*, &Klasa::x);
// std::cout << x.*break_in(nazwa_odwolamnia());

#include <GameClient/Initializer.h>

#endif //RTS_GAME_MACRO_H
