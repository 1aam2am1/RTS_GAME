#include <cinttypes>

struct MetaData::Reflect {
    const std::string_view name;

    const std::type_index type;

    const std::function<TPtr<Object>()> CreateInstance;

    const std::function<bool(Object *, Object *)> CopyInstance;

    const std::function<bool(Object *)> CheckInstance;

    const MetaData::flags_type &flags;

    Reflect(std::string_view name, std::type_index type,
            std::function<TPtr<Object>()> createInstance,
            std::function<bool(Object *, Object *)> copyInstance,
            std::function<bool(Object *)> checkInstance,
            const MetaData::flags_type &flags);
};

struct MetaData::ReflectFull : Reflect {
    const std::vector<std::pair<std::string_view, MetaData::TU>> getFields;
    const std::map<std::string_view, std::type_index> getTPtrType; //TODO: Remove if unnecessary

    ReflectFull(std::string_view name, std::type_index type,
                std::function<TPtr<Object>()> createInstance,
                std::function<bool(Object *, Object *)> copyInstance,
                std::function<bool(Object *)> checkInstance,
                const MetaData::flags_type &flags,
                std::vector<std::pair<std::string_view, MetaData::TU>> getFields,
                std::map<std::string_view, std::type_index> getTPtrType);
};

template<typename T>
struct MetaData::Register {

    template<typename Y>
    requires requires{
        requires is_base_of_template_v<Y, SGBase>;
        requires std::is_assignable<TU &, std::pair<std::function<void(
                typename Y::type)>, std::function<typename Y::type(void)>>>::value;
    }
    void registerMemberForSerialize(std::string_view str, Y T::*ptr) {
        auto func = [=](auto t) -> TU {
            using type = typename Y::type;
            std::function<void(type)> set;
            std::function<type()> get;

            if constexpr (std::is_assignable<decltype(t->*ptr), type>::value) {
                set = [=](type arg) { t->*ptr = arg; };
            }

            get = [=]() -> type { return (t->*ptr).get(); };

            return std::pair<std::function<void(type)>, std::function<type(void)>>{
                    set,
                    get
            };
        };

        add(str, func);
    }

    template<typename Y>
    requires requires{
        requires is_base_of_template_v<Y, SGBase>;
        requires !std::is_assignable<TU &, std::pair<std::function<void(
                typename Y::type)>, std::function<typename Y::type(void)>>>::value;
    }
    void registerMemberForSerialize(std::string_view str, Y T::*ptr) {
        auto func = [=](auto t) -> TU {
            std::function<void(nlohmann::json)> set;
            std::function<nlohmann::json()> get;

            if constexpr (std::is_assignable<decltype(t->*ptr), decltype(t->*ptr)>::value) {
                set = [=](nlohmann::json arg) { t->*ptr = arg.get<typename Y::type>(); };
            }

            get = [=]() -> nlohmann::json { return t->*ptr; };

            return std::pair<std::function<void(nlohmann::json)>, std::function<nlohmann::json(void)>>{
                    set,
                    get
            };
        };

        add(str, func);

        d.TPtr_type.emplace_back(str, typeid(typename Y::type));
    }

    template<typename Y>
    void registerMemberForSerialize(std::string_view str, TPtr<Y> T::*ptr) {
        auto func = [=](auto t) -> TU {
            using type = TPtr<Y>;
            std::function<void(type)> set;
            std::function<type()> get;

            if constexpr (std::is_assignable<decltype(t->*ptr), type>::value) {
                set = [=](type arg) { t->*ptr = arg; };
            }

            get = [=]() -> type { return t->*ptr; };

            return std::pair<std::function<void(type)>, std::function<type(void)>>{
                    set,
                    get
            };
        };

        add(str, func);

        d.TPtr_type.emplace_back(str, typeid(Y));
    }

    template<typename Y>
    void registerMemberForSerialize(std::string_view str, std::vector<TPtr<Y>> T::*ptr) {
        auto func = [=](auto t) -> TU {
            std::function<void(std::vector<TPtr<Object>>)> f;

            if constexpr (std::is_assignable<decltype(t->*ptr), std::vector<TPtr<Y>>>::value) {
                f = [=](std::vector<TPtr<Object>> arg) {
                    auto &vec = t->*ptr;
                    vec.clear();
                    vec.reserve(arg.size());
                    for (auto &e : arg) {
                        vec.emplace_back(dynamic_pointer_cast<Y>(e));
                    }
                };
            }

            return std::pair<
                    std::function<void(std::vector<TPtr<Object>>)>,
                    std::function<std::vector<TPtr<Object>>(void)>>{
                    f,
                    [=]() -> std::vector<TPtr<Object>> {
                        std::vector<TPtr<Object>> result;
                        auto &vec = t->*ptr;
                        result.reserve(vec.size());
                        for (auto &e : vec) {
                            result.emplace_back(static_pointer_cast<Object>(e));
                        }

                        return result;
                    }
            };
        };

        add(str, func);

        d.TPtr_type.emplace_back(str, typeid(Y));
    }

    template<typename Y>
    requires std::is_assignable<TU &, std::pair<
            std::function<void(std::vector<Y>)>,
            std::function<std::vector<Y>(void)>>>::value
    void registerMemberForSerialize(std::string_view str, std::vector<Y> T::*ptr) {
        auto func = [=](auto t) -> TU {
            std::function<void(std::vector<Y>)> set;
            std::function<std::vector<Y>()> get;

            if constexpr (std::is_assignable<decltype(t->*ptr), std::vector<Y>>::value) {
                set = [=](std::vector<Y> arg) { t->*ptr = arg; };
            }

            get = [=]() -> std::vector<Y> { return t->*ptr; };

            return std::pair<std::function<void(std::vector<Y>)>, std::function<std::vector<Y>(void)>>{
                    set,
                    get
            };
        };

        add(str, func);
    }

    template<typename Y>
    requires (!std::is_assignable<TU &, std::pair<
            std::function<void(std::vector<Y>)>,
            std::function<std::vector<Y>(void)>>>::value)
    void registerMemberForSerialize(std::string_view str, std::vector<Y> T::*ptr) {
        auto func = [=](auto t) -> TU {
            std::function<void(nlohmann::json)> set;
            std::function<nlohmann::json()> get;

            if constexpr (std::is_assignable<decltype(t->*ptr), decltype(t->*ptr)>::value) {
                set = [=](nlohmann::json arg) { arg.get_to(t->*ptr); };
            }

            get = [=]() -> nlohmann::json { return t->*ptr; };

            return std::pair<std::function<void(nlohmann::json)>, std::function<nlohmann::json(void)>>{
                    set,
                    get
            };
        };

        add(str, func);

        d.TPtr_type.emplace_back(str, typeid(Y));
    }

    template<typename Y>
    void registerMemberForSerialize(std::string_view str, Unity::GUID T::*ptr) {
        auto func = [=](auto t) -> TU {
            std::function<void(std::string)> f;

            if constexpr (std::is_assignable<decltype(t->*ptr), std::string>::value) {
                f = [=](std::string arg) { t->*ptr = arg; };
            }

            return std::pair<std::function<void(std::string)>, std::function<std::string(void)>>{
                    f,
                    [=]() -> std::string { return t->*ptr; }
            };
        };

        add(str, func);
    }

private:
    template<typename Z, typename M>
    static constexpr void registerMemberForSerializeStateMachineHelper(M &state, nlohmann::json &arg) {
        if (GameApi::demangle(typeid(Z).name()) == arg.get<std::string>()) {
            state.template transitionTo<Z>();
        }
    }

public:

    template<typename Y>
    requires is_base_of_template_v<Y, StateMachine>
    void registerMemberForSerialize(std::string_view str, Y T::*ptr) {
        auto func = [=](auto t) -> TU {
            std::function<void(nlohmann::json)> set;
            std::function<nlohmann::json()> get;

            if constexpr (!std::is_const<typename std::remove_reference<decltype(t->*ptr)>::type>::value) {
                set = [=](nlohmann::json arg) {
                    []<typename... Args>(StateMachine<Args...> &state, nlohmann::json &arg) {
                        (registerMemberForSerializeStateMachineHelper<Args>(state, arg), ...);
                    }(t->*ptr, arg);
                };
            }

            get = [=]() -> nlohmann::json {
                std::string name;
                (t->*ptr).visit([&name](auto &&state) { name = GameApi::demangle(typeid(*state).name()); });
                return name;
            };

            return std::pair<std::function<void(nlohmann::json)>, std::function<nlohmann::json(void)>>{
                    set,
                    get
            };
        };

        add(str, func);

        d.TPtr_type.emplace_back(str, typeid(Y));
    }

    template<typename Y>
    requires std::is_assignable<TU &, std::pair<
            std::function<void(Y)>,
            std::function<Y(void)>>>::value
    void registerMemberForSerialize(std::string_view str, Y T::*ptr) {
        auto func = [=](auto t) -> TU {
            std::function<void(Y)> set;
            std::function<Y()> get;

            if constexpr (std::is_assignable<decltype(t->*ptr), decltype(t->*ptr)>::value) {
                set = [=](Y arg) { t->*ptr = arg; };
            }

            get = [=]() -> Y { return t->*ptr; };

            return std::pair<std::function<void(Y)>, std::function<Y(void)>>{
                    set,
                    get
            };
        };

        add(str, func);
    }

    template<typename Y>
    requires requires{
        requires std::is_floating_point_v<Y>;
        requires !std::is_assignable<TU &, std::pair<std::function<void(Y)>, std::function<Y(void)>>>::value;
    }
    void registerMemberForSerialize(std::string_view str, Y T::*ptr) {
        auto func = [=](auto t) -> TU {
            std::function<void(double)> set;
            std::function<double()> get;

            if constexpr (std::is_assignable<decltype(t->*ptr), decltype(t->*ptr)>::value) {
                set = [=](double arg) { t->*ptr = arg; };
            }

            get = [=]() -> double { return t->*ptr; };

            return std::pair<std::function<void(double)>, std::function<double(void)>>{
                    set,
                    get
            };
        };

        add(str, func);
    }

    template<typename Y>
    requires requires {
        requires std::is_integral_v<Y>;
        requires (sizeof(Y) <= sizeof(int64_t));
        requires !std::is_assignable<TU &, std::pair<std::function<void(Y)>, std::function<Y(void)>>>::value;
    }
    void registerMemberForSerialize(std::string_view str, Y T::*ptr) {
        auto func = [=](auto t) -> TU {
            std::function<void(int64_t)> set;
            std::function<int64_t()> get;

            if constexpr (std::is_assignable<decltype(t->*ptr), decltype(t->*ptr)>::value) {
                set = [=](int64_t arg) { t->*ptr = arg; };
            }

            get = [=]() -> int64_t { return t->*ptr; };

            return std::pair<std::function<void(int64_t)>, std::function<int64_t(void)>>{
                    set,
                    get
            };
        };

        add(str, func);
    }

    template<typename Y>
    requires requires{
        requires !std::is_assignable<TU &, std::pair<std::function<void(Y)>, std::function<Y(void)>>>::value;
        requires !(std::is_integral_v<Y> && (sizeof(Y) <= sizeof(int64_t)));
        requires !std::is_floating_point_v<Y>;
        requires !is_base_of_template_v<Y, SGBase>;
        requires !is_base_of_template_v<Y, StateMachine>;
    }
    void registerMemberForSerialize(std::string_view str, Y T::*ptr) {
        auto func = [=](auto t) -> TU {
            std::function<void(nlohmann::json)> set;
            std::function<nlohmann::json()> get;

            if constexpr (std::is_assignable<decltype(t->*ptr), decltype(t->*ptr)>::value) {
                set = [=](nlohmann::json arg) { arg.get_to(t->*ptr); };
            }

            get = [=]() -> nlohmann::json { return t->*ptr; };

            return std::pair<std::function<void(nlohmann::json)>, std::function<nlohmann::json(void)>>{
                    set,
                    get
            };
        };

        add(str, func);

        d.TPtr_type.emplace_back(str, typeid(Y));
    }

    template<typename R>
    void registerMemberForSerialize(std::string_view str, void (T::*set)(R), R (T::*get)()) {
        auto func = [=](auto t) -> TU {
            std::function<void(R)> set;
            std::function<R(void)> get;

            if constexpr (std::is_invocable<decltype(set), decltype(t), R>::value) {
                set = [=](R arg) { t->*set(arg); };
            }

            if constexpr (std::is_invocable_r<R, decltype(get), decltype(t)>::value) {
                get = [=]() -> R { return t->*get(); };
            }

            return std::pair<std::function<void(R)>, std::function<R(void)>>{set, get};
        };

        add(str, func);
    }

    template<typename E>
    requires std::is_enum_v<E>
    void registerFlag(E e) {
        static_assert(sizeof(E) <= sizeof(uintmax_t), "Enum flag should be smaller or equal size, that we can store.");

        d.flags |= e;
/**
        auto it = d.flags.find(typeid(E));
        if (it != d.flags.end()) {
            it->second |= e;
        } else {
            d.flags[typeid(E)] = e;
        }*/
    }

    template<typename R>
    void registerMemberForSerialize(std::string_view str, std::function<void(T *, R)> set, std::function<R(T *)> get) {
        d.members.emplace_back(str, [=](Object *ob) -> TU {
            auto t = static_cast<T *>(ob);
            std::function<void(R)> s;
            std::function<R(void)> g;

            if (set) {
                s = [=](R arg) { set(t, arg); };
            }
            if (get) {
                g = [=]() -> R { return get(t); };
            }
            return std::pair<std::function<void(R)>, std::function<R(void)>>{s, g};
        });
    }

    template<typename Fun, typename Fun2>
    requires requires(Fun2 get){ get(std::declval<T *>()); }
    void registerMemberForSerialize(std::string_view str, Fun set, Fun2 get) {
        using ResultType = decltype(get(std::declval<T *>()));
        registerMemberForSerialize<ResultType>(str, set, get);
    }

    template<typename F>
    constexpr void add(std::string_view str, F func) {
        d.members.emplace_back(str, [=](Object *ob) -> TU {
            auto t = static_cast<T *>(ob);
            return func(t);
        });

        d.c_members.emplace_back(str, [=](const Object *ob) -> TU {
            auto t = static_cast<const T *>(ob);
            return func(t);
        });
    }

    MetaData::Data &d;
};

template<typename T>
auto MetaData::register_class(std::string_view str) {
    return register_class<T>(str, []() {
        if constexpr (!std::is_constructible_v<T>) {
            GameApi::log(ERR.fmt("Tried creating object of abstract type %s",
                                 GameApi::demangle(typeid(T).name()).data()));
            return TPtr<T>{};
        } else {
            return TPtr<T>{std::make_shared<T>()};
        }
    });
}

template<typename T>
auto MetaData::register_class(std::string_view str, std::function<TPtr<T>()> constructor) {
    static_assert(std::is_base_of_v<Object, T>, "only subclasses, please");

    auto it = reflection.find(typeid(T));

    if (it != reflection.end()) {
        if (it->second.name != str) {
            GameApi::log(
                    ERR.fmt("Class with name: %s tried redeclared with different name. Redeclared name: %s -> %s",
                            GameApi::demangle(it->first.name()).data(),
                            it->second.name.data(),
                            str.data()));
        }
        if (constructor && !it->second.create) {
            it->second.create = constructor;
        }
        return Register < T > {it->second};
    }

    auto[it2, b] = name_type.emplace(str, typeid(T));

    if (!b) {
        GameApi::log(ERR.fmt("Classes with type: %s %s redeclared with the same name. %s",
                             GameApi::demangle(it2->second.name()).data(),
                             GameApi::demangle(typeid(T).name()).data(),
                             str.data()));
    }

    Data result;
    result.name = str;
    result.create = constructor;
    result.copy = [](auto f1, auto f2) {
        if constexpr(!std::is_assignable_v<T &, const T &>) {
            return false;
        } else {
            auto left = dynamic_cast<T *>(f1);
            auto right = dynamic_cast<T *>(f2);
            if (left && right) {
                *left = *right;
                return true;
            } else {
                return false;
            }
        }
    };
    result.check = [](auto o) {
        return dynamic_cast<const T *>(o);
    };

    auto[it3, b2] = reflection.emplace(typeid(T), result);

    return Register < T > {it3->second};
}

/////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void Importers::register_importer(std::string_view extension) {
    auto[it2, b] = ext_importer.emplace(extension, typeid(T));

    if (!b) {
        GameApi::log(ERR.fmt("Classes with type: %s %s import the same extension. %s",
                             GameApi::demangle(it2->second.name()).data(),
                             GameApi::demangle(typeid(T).name()).data(),
                             extension.data()));
    }

    auto it = MetaData::getReflection(typeid(T));

    if (!it.CreateInstance) {
        GameApi::log(
                ERR.fmt("Importer with name: %s not declared. Extension: %s",
                        GameApi::demangle(typeid(T).name()).data(),
                        extension.data()));
    }
}

template<typename T>
void Importers::register_importer(int64_t priority) {
    auto[it2, b] = ext_priority.emplace(typeid(T), priority);

    if (!b) {
        GameApi::log(ERR.fmt("Class with type: %s tried change. Priority: %" PRIi64,
                             GameApi::demangle(typeid(T).name()).data(),
                             priority));
    }

    auto it = MetaData::getReflection(typeid(T));

    if (!it.CreateInstance) {
        GameApi::log(
                ERR.fmt("Importer with name: %s not declared. Priority: %" PRIi64,
                        GameApi::demangle(typeid(T).name()).data(),
                        priority));
    }
}

struct Enums::Reflect {
    const Data &d;
};

template<typename T>
struct Enums::Register {

    void setReflection(std::initializer_list<std::pair<T, std::string_view>> l) {
        std::vector<std::string_view> m;
        m.reserve(l.size());
        for (auto &&i : l) {
            m.emplace_back(i.second);
        }
        d.members = std::move(m);
    }

    Data &d;
};

template<typename T>
requires std::is_enum<T>::value
auto Enums::register_enum(std::string_view) {
    auto it = reflection.find(typeid(T));
    if (it != reflection.end()) {
        return Register < T > {it->second};
    }

    Data result;

    auto[it2, b2] = reflection.emplace(typeid(T), result);

    return Register < T > {it2->second};
}

namespace {
    template<class T>
    constexpr std::string_view get_name() {
        char const *p = __PRETTY_FUNCTION__;
        while (*p++ != '=');
        for (; *p == ' '; ++p);
        char const *p2 = p;
        int count = 1;
        bool ret = false;
        for (;; ++p2) {
            switch (*p2) {
                case '[':
                    ++count;
                    break;
                case ']':
                    --count;
                    if (!count) {
                        if (ret) {
                            while (*p2 != ';') { p2--; }
                        }

                        return {p, std::size_t(p2 - p)};
                    }
                    break;
                case '=':
                    ret = true;
            }
        }
        return {p};
    }
}

template<typename Z>
static constexpr void register_stStateMachineHelper(std::vector<std::string_view> &arg) {
    arg.emplace_back(get_name<Z>());
}

template<typename T>
requires is_base_of_template_v<T, StateMachine>
void Enums::register_st(std::string_view) {
    auto it = reflection.find(typeid(T));
    if (it != reflection.end()) { return; }

    auto &object = reflection[typeid(T)];
    []<typename... Args>(StateMachine<Args...> *, Enums::Data &arg) {
        (register_stStateMachineHelper<Args>(arg.members), ...);
    }((T *) (nullptr), object);

}
