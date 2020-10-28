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

    template<typename T>
    static Component *AddComponent(GameObject *gameObject);

    /// Creates a new GameObject.
    static GameObject *CreateGameObject(std::string name, std::vector<std::type_info> types);

    /// Create a new instance of the given type.
    static Object *CreateInstance(std::type_info type);

    template<typename T>
    static Object *CreateInstance();

    /// Create a new instance of the given type.
    template<typename T>
    static T *CreateInstance();

public:

    template<typename T>
    static auto &register_class(std::string_view str);

#ifndef TEST
    private:
#endif

    friend class SerializerBase;

    using TU = std::variant<int64_t *, double *, std::string *, TPtr<Object> *>;
    using CTU = std::variant<const int64_t *, const double *, const std::string *, const TPtr<Object> *>;
    using ST = std::vector<std::pair<std::string_view, TU>>;
    using CST = std::vector<std::pair<std::string_view, CTU>>;

    struct Reflect {
        virtual CST get(const Object *) const = 0;

        virtual ST get(Object *) const = 0;

        /// Check if object is of given type, exist mapped values
        /// \return
        virtual bool check(const Object *) = 0;

        virtual TPtr<Object> create() = 0;
    };

    template<typename T>
    class Register : public Reflect {
    public:
        using TT = std::variant<int64_t T::*, double T::*, std::string T::*, TPtr<Object> T::*>;

        template<typename Y>
        void registerMemberForSerialize(std::string_view str, Y T::*ptr) {
            v.emplace_back(str, ptr);
        }

        CST get(const Object *o) const override {
            CST result;
            auto ptr = dynamic_cast<const T *>(o);
            if (!ptr) {
                return result;
            }

            result.reserve(v.size());

            for (auto &it:v) {
                result.emplace_back(it.first, std::visit([ptr](auto &&arg) -> CTU { return &((*ptr).*arg); },
                                                         it.second));
            }

            return result;
        }

        ST get(Object *o) const override {
            ST result;
            auto ptr = dynamic_cast<T *>(o);
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

        bool check(const Object *o) override {
            return dynamic_cast<const T *>(o);
        }

        TPtr<Object> create() override {
            return TPtr<Object>{nullptr, std::make_shared<T>()};
        }

    private:
        std::vector<std::pair<std::string_view, TT>> v{};
    };

    //TODO: Change reflection to type->name, list of valid reflections and one constructor
    //info -> name, reflection list {name, pointer}
    static std::map<std::type_index, std::pair<const std::string_view, const std::shared_ptr<Reflect>>> reflection;
    //name -> info
    static std::map<std::string_view, const std::type_index> name_type;
};

template<typename T>
auto &ObjectFactory::register_class(std::string_view str) {
    auto it = reflection.find(typeid(T));

    if (it != reflection.end()) {
        if (it->second.first != str) {
            GameApi::log(ERR.fmt("Class with name: %s tried redeclaring with different name. Redeclared name: %s -> %s",
                                 GameApi::demangle(it->first.name()).data(),
                                 it->second.first.data(),
                                 str.data()));
        }
        return *static_cast<Register<T> *>(it->second.second.get());
    }

    auto[it2, b] = name_type.emplace(str, typeid(T));

    if (!b) {
        GameApi::log(ERR.fmt("Classes with type: %s %s redeclared with the same name. %s",
                             GameApi::demangle(it2->second.name()).data(),
                             GameApi::demangle(typeid(T).name()).data(),
                             str.data()));
    }

    auto pointer = std::static_pointer_cast<Reflect>(std::make_shared<Register<T>>());
    reflection.emplace(typeid(T), std::pair<const std::string_view, const std::shared_ptr<Reflect>>{str, pointer});
    return *static_cast<Register<T> *>(pointer.get());
}


#endif //RTS_GAME_OBJECTFACTORY_H
