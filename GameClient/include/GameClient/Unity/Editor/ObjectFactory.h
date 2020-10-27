//
// Created by Michal_Marszalek on 27.10.2020.
//

#ifndef RTS_GAME_OBJECTFACTORY_H
#define RTS_GAME_OBJECTFACTORY_H

#include <GameClient/Unity/Core/GameObject.h>
#include <typeinfo>
#include <unordered_map>
#include <map>
#include <typeindex>

class ObjectFactory {
public:

    /// Creates a new component and adds it to the specified GameObject.
    static Component *AddComponent(GameObject *gameObject, std::type_info type);

    /// Creates a new GameObject.
    static GameObject *CreateGameObject(std::string name, std::vector<std::type_info> types);

    /// Create a new instance of the given type.
    static Object *CreateInstance(std::type_info type);

    /// Create a new instance of the given type.
    template<typename T>
    static T *CreateInstance();

public:

    template<typename T>
    static auto &register_class(std::string_view str);

#ifndef TEST
    private:
#endif

    friend class Serialize;

    using TU = std::variant<int64_t *, double *, std::string *, Object *>;
    using ST = std::vector<std::pair<std::string_view, TU>>;

    struct Reflect {
        virtual ST get(Object *) = 0;
    };

    template<typename T>
    class Register : public Reflect {
    public:
        using TT = std::variant<int64_t T::*, double T::*, std::string T::*, Object T::*>;

        template<typename Y>
        void registerMemberForSerialize(std::string_view str, Y T::*ptr) {
            v.emplace_back(str, ptr);
        }

        ST get(Object *o) override {
            ST result;
            T *ptr = dynamic_cast<T *>(o);
            if (!ptr) {
                return result;
            }

            result.reserve(v.size());

            for (auto &it:v) {
                result.emplace_back(it.first, std::visit([ptr](auto &&arg) -> TU { return &((*ptr).*arg); },
                                                         it.second));
            }

            return result;
        }

    private:
        std::vector<std::pair<std::string_view, TT>> v{};
    };

    //info, name -> reflection list {name, pointer}
    static std::map<std::pair<std::type_index, std::string_view>, const std::shared_ptr<Reflect>> reflection;
};

template<typename T>
auto &ObjectFactory::register_class(std::string_view str) {
    auto it = std::find_if(reflection.begin(), reflection.end(), [](auto &it) {
        return it.first.first == typeid(T);
    });

    if (it != reflection.end()) {
        if (it->first.second != str) {
            GameApi::log(ERR.fmt("Class with name: %s tried redeclaring with different name. Redeclared name: %s -> %s",
                                 GameApi::demangle(it->first.first.name()).data(),
                                 it->first.second.data(),
                                 str.data()));
        }
        return *dynamic_cast<Register<T> *>(it->second.get());
    }

    it = std::find_if(reflection.begin(), reflection.end(), [str](auto &it) {
        return it.first.second == str;
    });

    if (it != reflection.end()) {
        GameApi::log(ERR.fmt("Classes with name: %s %s redeclared with the same name. %s",
                             GameApi::demangle(it->first.first.name()).data(),
                             GameApi::demangle(typeid(T).name()).data(),
                             str.data()));
    }

    auto pointer = std::static_pointer_cast<Reflect>(std::make_shared<Register<T>>());
    return *dynamic_cast<Register<T> *>(reflection.emplace(std::pair<std::type_index, std::string_view>{typeid(T), str},
                                                           pointer).first->second.get());
}


#endif //RTS_GAME_OBJECTFACTORY_H
