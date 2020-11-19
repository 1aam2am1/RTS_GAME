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


#define REGISTER_MEMBER_FOR_SERIALIZE(arg, TYPE) registerMemberForSerialize(RMFS_SERIALIZE(TYPE, UNPAREN(arg)))
#define REGISTER_IMPORTER_EXTENSION(arg, TYPE) register_importer<TYPE>(arg)
//endregion

#define UNIQUE_ID(PRE) CONCATENATE(PRE, __COUNTER__)


/// TODO: Set TYPE SO THAT you can set to don't include base classes in serialization TYPE or (TYPE, PRIVATE) or (TYPE, OBJECT, ...)
/// Export class to save it in scene and use in in gameobject as component
/// What should be serialized
#define EXPORT_CLASS(TYPE, ...)                     \
namespace {                                         \
    static int INTERNAL_NO_USE_CLASS_##TYPE = Initializer::add([](){ \
        [[maybe_unused]] auto& t = MetaData::register_class<TYPE>(#TYPE); \
        FOR_EACH(t.REGISTER_MEMBER_FOR_SERIALIZE, TYPE, __VA_ARGS__) \
        return 0;                                   \
    });                                            \
}


/// Export class to import and export classes
#define EXPORT_IMPORTER(TYPE, EXT, ...) \
namespace {                        \
    static int INTERNAL_NO_USE_IMPORTER_##TYPE = Initializer::add([](){ \
        static_assert(std::is_base_of_v<AssetImporter, TYPE>, "only subclasses, please"); \
        [[maybe_unused]] auto& t = MetaData::register_class<TYPE>(#TYPE);                \
        FOR_EACH(t.REGISTER_MEMBER_FOR_SERIALIZE, TYPE, __VA_ARGS__)    \
        FOR_EACH(MetaData::REGISTER_IMPORTER_EXTENSION, TYPE, UNPAREN(EXT))     \
        return 0;                               \
    });                                   \
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
/// \param ... priority
#define MENU_ITEM(FUNC, PATH, ...) \
namespace {                        \
    static int UNIQUE_ID(INTERNAL_NO_USE_F_) = Initializer::add([](){ \
        Menu::addItem(PATH, FUNC __VA_OPT__(,) __VA_ARGS__); \
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



/// Define break in pointer that could break in inside the private class
#define DEFINE_BREAK_IN(name, t, member)\
           struct name {\
             using type = t;\
             friend constexpr t break_in(name);\
           };\
           template struct break_in_<name, member>

//DEFINE_BREAK_IN(nazwa_odwolamnia, int Klasa::*, &Klasa::x);
// std::cout << x.*break_in(nazwa_odwolamnia());

#define EXCEPTION_PRINT                     \
catch (const std::exception &e) {           \
    GameApi::log(ERR.fmt("%s", e.what()));  \
}

#include <GameClient/Initializer.h>
#include <GameClient/MetaData.h>
#include <GameApi/Config.h>

#endif //RTS_GAME_MACRO_H
