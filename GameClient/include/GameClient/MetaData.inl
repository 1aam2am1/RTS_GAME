#include <cinttypes>

struct MetaData::Reflect {
    const std::string_view name;

    const std::type_index type;

    const std::function<TPtr<Object>()> CreateInstance;

    const std::function<bool(Object *, Object *)> CopyInstance;

    Reflect(const std::string_view name, const std::type_index type,
            const std::function<TPtr<Object>()> &createInstance,
            const std::function<bool(Object *, Object *)> &copyInstance);
};

struct MetaData::ReflectFull : Reflect {
    const std::vector<std::pair<std::string_view, MetaData::TU>> getFields;

    ReflectFull(const std::string_view name, const std::type_index type,
                const std::function<TPtr<Object>()> &createInstance,
                const std::function<bool(Object *, Object *)> &copyInstance,
                const std::vector<std::pair<std::string_view, MetaData::TU>> &getFields);
};

template<typename T>
struct MetaData::Register {
    template<typename Y>
    void registerMemberForSerialize(std::string_view str, Y T::*ptr) {
        auto func = [=](auto t) -> TU {
            auto def = [&]() {
                std::function<void(Y)> set;
                std::function<Y()> get;

                if constexpr (std::is_assignable<decltype(t->*ptr), Y>::value) {
                    set = [=](Y arg) { t->*ptr = arg; };
                }

                get = [=]() -> Y { return t->*ptr; };

                return std::pair<std::function<void(Y)>, std::function<Y(void)>>{
                        set,
                        get
                };
            };

            if constexpr (std::is_same_v<Y, Unity::GUID>) {
                std::function<void(std::string)> f;

                if constexpr (std::is_assignable<decltype(t->*ptr), std::string>::value) {
                    f = [=](std::string arg) { t->*ptr = arg; };
                }

                return std::pair<std::function<void(std::string)>, std::function<std::string(void)>>{
                        f,
                        [=]() -> std::string { return t->*ptr; }
                };
            } else if constexpr (is_instance_v<Y, std::vector>) {
                using object_type = typename Y::value_type;
                if constexpr(is_instance_v<object_type, TPtr>) {
                    std::function<void(std::vector<TPtr<Object>>)> f;

                    if constexpr (std::is_assignable<decltype(t->*ptr), Y>::value) {
                        f = [=](std::vector<TPtr<Object>> arg) {
                            auto &vec = t->*ptr;
                            vec.clear();
                            vec.reserve(arg.size());
                            for (auto &e : arg) {
                                vec.emplace_back(dynamic_pointer_cast<typename object_type::element_type>(e));
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

                } else {
                    if constexpr(std::is_assignable<TU, decltype(def())>::value) {
                        return def();
                    } else {
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
                    }
                }
            } else if constexpr (is_instance_v<Y, SetterGetter>) {
                using type = typename Y::type;
                std::function<void(type)> set;
                std::function<type()> get;

                if constexpr (std::is_assignable<decltype(t->*ptr), type>::value) {
                    set = [=](type arg) { t->*ptr = arg; };
                }

                get = [=]() -> nlohmann::json { return (t->*ptr).get(); };

                return std::pair<std::function<void(type)>, std::function<type(void)>>{
                        set,
                        get
                };
            } else {
                if constexpr(std::is_assignable<TU, decltype(def())>::value) {
                    return def();
                } else {
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
                }
            }
        };

        d.members.emplace_back(str, [=](Object *ob) -> TU {
            auto t = static_cast<T *>(ob);
            return func(t);
        });

        d.c_members.emplace_back(str, [=](const Object *ob) -> TU {
            auto t = static_cast<const T *>(ob);
            return func(t);
        });
    }

    template<typename R>
    void registerMemberForSerialize(std::string_view str, void (T::*set)(R), R (T::*get)()) {
        d.members.emplace_back(str, [=](Object *ob) -> TU {
            auto t = static_cast<T *>(ob);
            std::function<void(R)> s;
            std::function<R(void)> g;

            if (set) {
                s = [=](R arg) { t->*set(arg); };
            }
            if (get) {
                g = [=]() -> R { return t->*get(); };
            }
            return std::pair<std::function<void(R)>, std::function<R(void)>>{s, g};
        });

    }

    MetaData::Data &d;
};

template<typename T>
auto MetaData::register_class(std::string_view str) {
    return register_class<T>(str, []() {
        if constexpr (std::is_abstract_v<T>) {
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
    result.create = [constructor]() -> TPtr<> {
        if (!constructor) {
            GameApi::log(ERR.fmt("Tried creating object with empty constructor function"));
            return {};
        } else {
            return constructor();
        }
    };
    result.copy = [](auto f1, auto f2) {
        auto left = dynamic_cast<T *>(f1);
        auto right = dynamic_cast<T *>(f2);
        if (left && right) {
            *left = *right;
            return true;
        } else {
            return false;
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
