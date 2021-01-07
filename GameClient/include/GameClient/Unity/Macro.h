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

//region REGISTER_MEMBER_FOR_SERIALIZE
#define RMFS_SERIALIZE_1(type, arg) STRINGIZE(arg), break_in(type##_##arg())
#define RMFS_SERIALIZE_2(type, name, arg) name, break_in(type##_##arg())
#define RMFS_SERIALIZE_3(type, name, set, get) name, UNPAREN(set), UNPAREN(get)

#define RMFS_SERIALIZE_(N, type, ...) CONCATENATE(RMFS_SERIALIZE_, N)(type, __VA_ARGS__)
#define RMFS_SERIALIZE(type, ...) RMFS_SERIALIZE_(FOR_EACH_NARG(__VA_ARGS__), type, __VA_ARGS__)


#define REGISTER_MEMBER_FOR_SERIALIZE(arg, TYPE) registerMemberForSerialize(RMFS_SERIALIZE(TYPE, UNPAREN(arg)))
#define REGISTER_IMPORTER_EXTENSION(arg, TYPE) register_importer<TYPE>(arg)
//endregion

//region REGISTER_MEMBER_FOR_PRIVATE_BREAK
#define RMFS_SERIALIZE_PRIVATE_BREAK_1(type, arg) DEFINE_BREAK_IN(type, type##_##arg, &type::arg)
#define RMFS_SERIALIZE_PRIVATE_BREAK_2(type, name, arg) DEFINE_BREAK_IN(type, type##_##arg, &type::arg)
#define RMFS_SERIALIZE_PRIVATE_BREAK_3(type, name, set, get)

#define RMFS_SERIALIZE_PRIVATE_BREAK_(N, type, ...) CONCATENATE(RMFS_SERIALIZE_PRIVATE_BREAK_, N)(type, __VA_ARGS__)
#define RMFS_SERIALIZE_PRIVATE_BREAK(type, ...) RMFS_SERIALIZE_PRIVATE_BREAK_(FOR_EACH_NARG(__VA_ARGS__), type, __VA_ARGS__)

#define REGISTER_MEMBER_FOR_PRIVATE_BREAK(arg, TYPE) RMFS_SERIALIZE_PRIVATE_BREAK(TYPE, UNPAREN(arg))
//endregion
//endregion

#define UNIQUE_ID(PRE) CONCATENATE(PRE, __COUNTER__)

#define INITIALIZE_FUNC(...)                        \
namespace {                                         \
    static int UNIQUE_ID(INITIALIZE_FUNC_USE_F_) = Initializer::add([]() { \
        __VA_ARGS__;                                \
    });                                             \
}

/// TODO: Set TYPE SO THAT you can set to don't include base classes in serialization TYPE or (TYPE, PRIVATE) or (TYPE, OBJECT, ...)
/// Export class to save it in scene and use in in gameobject as component
/// \param TYPE TYPE of class
/// \param ... What should be serialized
#define EXPORT_CLASS(TYPE, ...)                                                 \
namespace {                                                                     \
    DEFINE_BREAK_IN_CLASS(TYPE)                                                 \
    FOR_EACH(REGISTER_MEMBER_FOR_PRIVATE_BREAK, TYPE, __VA_ARGS__)              \
    static int INTERNAL_NO_USE_CLASS_##TYPE = Initializer::add([](){            \
        [[maybe_unused]] auto t = MetaData::register_class<TYPE>(#TYPE);        \
        FOR_EACH(t.REGISTER_MEMBER_FOR_SERIALIZE, TYPE, __VA_ARGS__)            \
    });                                                                         \
}

/// Export class to save it in scene and use in in gameobject as component
/// \param TYPE TYPE of class
/// \param ... What should be serialized
#define EXPORT_CLASS_CONSTRUCTOR(TYPE, CONSTRUCTOR, ...)                    \
namespace {                                                                 \
    DEFINE_BREAK_IN_CLASS(TYPE)                                             \
    FOR_EACH(REGISTER_MEMBER_FOR_PRIVATE_BREAK, TYPE, __VA_ARGS__)          \
    static int INTERNAL_NO_USE_CLASS_##TYPE = Initializer::add([](){        \
        [[maybe_unused]] auto t = MetaData::register_class<TYPE>(#TYPE, CONSTRUCTOR);    \
        FOR_EACH(t.REGISTER_MEMBER_FOR_SERIALIZE, TYPE, __VA_ARGS__)        \
    });                                                                     \
}


/// Export class to import and export classes
#define EXPORT_IMPORTER(TYPE, EXT, ...) \
EXPORT_CLASS(TYPE, __VA_ARGS__)         \
namespace {                             \
    static int INTERNAL_NO_USE_IMPORTER_##TYPE = Initializer::add([](){ \
        static_assert(std::is_base_of_v<AssetImporter, TYPE>, "only subclasses, please"); \
        FOR_EACH(Importers::REGISTER_IMPORTER_EXTENSION, TYPE, UNPAREN(EXT))     \
        return 0;                               \
    });                                   \
}

/// Export class to save it in scene and allows you to place a
/// script anywhere in the "Component" menu, instead of just the "Component->Scripts" menu.
/// \param TYPE class type
/// \param PATH PATH and name of where to place component
/// \param ... priority
#define ADD_COMPONENT_MENU(TYPE, ...)                       \
EXPORT_CLASS(TYPE, __VA_ARGS__)                             \
CONTEXT_MENU(Component, #TYPE, [](){                        \
    if (Selection::activeGameObject) {                      \
        Selection::activeGameObject->AddComponent<TYPE>();  \
    }                                                       \
})


/// Mark a ScriptableObject-derived type to be automatically listed in the Assets/Create submenu,
/// so that instances of the type can be easily created and stored in the project as ".asset" files.
#define CREATE_ASSET_MENU(TYPE, ...) \
EXPORT_CLASS(TYPE, __VA_ARGS__)      \
namespace {                                         \
    static int INTERNAL_NO_USE_CLASS_##TYPE = Initializer::add([](){ \
        MetaData::register_asset<TYPE>()            \
        return 0;                                   \
    });                                             \
}

/// The MenuItem attribute allows you to add menu items to the main menu and inspector context menus.
/// \param FUNC void() or void(MenuCommand) or (validation) bool() or bool(MenuCommand
/// \param PATH -> PATH OR CONTEXT/CLASS_NAME/FUNC NAME
/// \param ... priority
#define MENU_ITEM(FUNC, PATH, ...) \
namespace {                        \
    static int UNIQUE_ID(INTERNAL_NO_USE_F_) = Initializer::add([](){ \
        Menu::addItem(PATH, UNPAREN(FUNC) __VA_OPT__(,) __VA_ARGS__); \
        return 0;                 \
    });                          \
}

/// The ContextMenu attribute allows you to add commands to the context menu of this class.
/// \param TYPE class type
/// \param FUNC void() or void(MenuCommand) or (validation) bool() or bool(MenuCommand
/// \param ... validation, priority
#define CONTEXT_MENU(TYPE, PATH, FUNC, ...) \
MENU_ITEM(FUNC, "CONTEXT/" #TYPE "/" PATH, __VA_ARGS__)

/// The MenuItem attribute allows you to add tabs to menu
/// \param ... priority
#define MENU_TAB(PATH, ...) \
namespace {                 \
    static const int UNIQUE_ID(INTERNAL_NO_USE_T_) = Initializer::add([](){ \
        Menu::addPlaceHolder(PATH __VA_OPT__(,) __VA_ARGS__);   \
        return 0;                 \
    });                          \
}

/// Tells an Editor class which run-time type it's an editor for.
/// \param isFallback	If true, match this editor only if all non-fallback editors do not match. Defaults to false.
/// \param editorForChildClasses  If true, child classes of inspectedType will also show this editor. Defaults to false.
#define CUSTOM_EDITOR(EDITOR, TYPE, ...)   \
namespace {                        \
    static int INTERNAL_NO_USE_CUSTOM_EDITOR_##EDITOR = Initializer::add([](){ \
        Editor::addCustom<EDITOR, TYPE>(__VA_ARGS__);       \
        return 0;                               \
    });                                   \
}
#define CUSTOM_EDITOR_FALLBACK(EDITOR, TYPE, ...)   \
namespace {                        \
    static int INTERNAL_NO_USE_CUSTOM_EDITOR_##EDITOR = Initializer::add([](){ \
        Editor::addCustomFallback<EDITOR, TYPE>(__VA_ARGS__);       \
        return 0;                               \
    });                                   \
}


#define DEFINE_BREAK_IN_CLASS(type)         \
template<typename Tag, auto Member>         \
struct break_in_##type {                    \
    friend constexpr auto break_in(Tag) {   \
        return Member;                      \
    }                                       \
};

/// Define break in pointer that could break in inside the private class
#define DEFINE_BREAK_IN(type, name, member)\
           struct name {\
             friend constexpr auto break_in(name);\
           };\
           template struct break_in_##type<name, member>;

//DEFINE_BREAK_IN(type_in_class_break, nazwa_odwolamnia, &Klasa::x);
// std::cout << x.*break_in(nazwa_odwolamnia());

#define EXCEPTION_PRINT                     \
catch (const std::exception &e) {           \
    GameApi::log(ERR.fmt("%s", e.what()));  \
}

#undef IM_ASSERT
#define IM_ASSERT(_EXPR)  do{ if(!(_EXPR)){ GameApi::log( ERR.fmt("%s", "Expression: " #_EXPR)); std::terminate();}}while(0)

#include <GameClient/Initializer.h>
#include <GameClient/MetaData.h>
#include <GameApi/Config.h>

#endif //RTS_GAME_MACRO_H
