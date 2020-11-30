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
        d.members.emplace_back(str, [=](Object *ob) -> TU {
            auto t = static_cast<T *>(ob);
            if constexpr (std::is_same_v<Y, Unity::GUID>) {
                std::function<void(std::string)> f;

                if constexpr (std::is_assignable<decltype(t->*ptr), std::string>::value) {
                    f = [=](std::string arg) { t->*ptr = arg; };
                }

                return std::pair<std::function<void(std::string)>, std::function<std::string(void)>>{
                        f,
                        [=]() -> std::string { return t->*ptr; }
                };
            } else {
                std::function<void(Y)> f;

                if constexpr (std::is_assignable<decltype(t->*ptr), Y>::value) {
                    f = [=](Y arg) { t->*ptr = arg; };
                }

                return std::pair<std::function<void(Y)>, std::function<Y(void)>>{
                        f,
                        [=]() -> Y { return t->*ptr; }
                };
            }
        });

        d.c_members.emplace_back(str, [=](const Object *ob) -> TU {
            auto t = static_cast<const T *>(ob);
            if constexpr (std::is_same_v<Y, Unity::GUID>) {
                return std::pair<std::function<void(std::string)>, std::function<std::string(void)>>(
                        std::function<void(std::string)>{},
                        [=]() -> std::string { return t->*ptr; }
                );
            } else {
                return std::pair<std::function<void(Y)>, std::function<Y(void)>>{
                        std::function<void(Y)>{},
                        [=]() -> Y { return t->*ptr; }
                };
            }
        });
    }

    template<typename R>
    void registerMemberForSerialize(std::string_view str, void (T::*set)(R), R (T::*get)()) {
        d.members.emplace_back(str, [=](Object *ob) -> TU {
            auto t = static_cast<T *>(ob);
            return std::pair<std::function<void(R)>, std::function<R(void)>>{
                    [=](R arg) { t->*set(arg); },
                    [=]() -> R { return t->*get(); }
            };
        });

    }

    MetaData::Data &d;
};

template<typename T>
auto MetaData::register_class(std::string_view str) {
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
    result.create = []() {
        if constexpr (std::is_abstract_v<T>) {
            GameApi::log(ERR.fmt("Tried creating object of abstract type %s",
                                 GameApi::demangle(typeid(T).name()).data()));
            return TPtr<Object>{nullptr};
        } else {
            return TPtr<Object>{nullptr, std::make_shared<T>()};
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
        GameApi::log(ERR.fmt("Class with type: %s tried change. Priority: %lli",
                             GameApi::demangle(typeid(T).name()).data(),
                             priority));
    }

    auto it = MetaData::getReflection(typeid(T));

    if (!it.CreateInstance) {
        GameApi::log(
                ERR.fmt("Importer with name: %s not declared. Priority: %lli",
                        GameApi::demangle(typeid(T).name()).data(),
                        priority));
    }
}
