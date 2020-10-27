//
// Created by Michal_Marszalek on 23.09.2020.
//

#ifndef RTS_GAME_MACRO_H
#define RTS_GAME_MACRO_H

//region STRINGIZE
#define REGISTER_MEMBER_FOR_SERIALIZE(arg, ...) registerMemberForSerialize(STRINGIZE(arg), __VA_ARGS__::arg)
#define STRINGIZE_ARG(arg) STRINGIZE(arg), arg
#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2
//endregion

//region FOR_EACH
#define FOR_EACH_0(what, y, x, ...)
#define FOR_EACH_1(what, y, x, ...) what(x, y);
#define FOR_EACH_2(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_1(what,y,   __VA_ARGS__)
#define FOR_EACH_3(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_2(what, y, __VA_ARGS__)
#define FOR_EACH_4(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_3(what, y,  __VA_ARGS__)
#define FOR_EACH_5(what, y, x, ...)\
  what(x, y);\
 FOR_EACH_4(what, y,  __VA_ARGS__)
#define FOR_EACH_6(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_5(what, y,  __VA_ARGS__)
#define FOR_EACH_7(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_6(what, y,  __VA_ARGS__)
#define FOR_EACH_8(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_7(what, y,  __VA_ARGS__)

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__ __VA_OPT__(,) FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define FOR_EACH_RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, x, ...) CONCATENATE(FOR_EACH_, N)(what, x, __VA_ARGS__)
#define FOR_EACH(what, x, ...) FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, x, ## __VA_ARGS__)
//endregion

#define UNIQUE_ID(PRE) CONCATENATE(PRE, __COUNTER__)

#define TPTR_P(NAME) TPtr<Object> NAME{this}
#define TPTR_PT(TYPE, NAME) TPtr<TYPE> NAME{this}

/// Export class to save it in scene and use in in gameobject as component
/// What should be serialized
#define EXPORT_CLASS(TYPE, ...)                     \
namespace {                                         \
    static int INTERNAL_NO_USE_CLASS_##TYPE = [](){ \
        auto& t = ObjectFactory::register_class<TYPE>(#TYPE); \
        FOR_EACH(t.REGISTER_MEMBER_FOR_SERIALIZE, &TYPE, __VA_ARGS__) \
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


/// Define break in pointer that could break in inside the private class
#define DEFINE_BREAK_IN(name, t, member)\
           struct name {\
             using type = t;\
             friend constexpr t break_in(name);\
           };\
           template struct break_in_<name, member>

//DEFINE_BREAK_IN(nazwa_odwolamnia, int Klasa::*, &Klasa::x);
// std::cout << x.*break_in(nazwa_odwolamnia());


#endif //RTS_GAME_MACRO_H
