
template<typename... Args>
TPtr<GameObject> newGameObject(std::string name) {
    auto i = newGameObject(name);

    using array = int[sizeof...(Args)];
    array{(i->AddComponent<Args>(), 1)...};

    return i;
}

template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
std::vector<TPtr<T>> GameObject::GetComponents() const {
    if constexpr (std::is_same_v<T, Component>) {
        return components;
    } else {
        std::vector<TPtr<T>> result;
        result.reserve(components.size());
        for (auto &c : components) {
            auto p = dynamic_pointer_cast<T>(c);
            if (p) {
                result.emplace_back(p);
            }
        }

        return result;
    }
}

HAS_FIELD(Awake)

template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
TPtr<T> GameObject::AddComponent() {
    if constexpr (std::is_base_of_v<Transform, T>) {
        if (!components.empty()) {
            GameApi::log(ERR.fmt("Transform should be one for game object"));
            return transform();
        }
    } else {
        if (components.empty()) {
            auto discard = transform(); //< Adds transform to game object
        }
    }
    //TODO: Check meta if there is flag only one and then check if there exists
    auto result = components.emplace_back(std::make_shared<T>());

    //TODO: !!! Awake of Component when game running !!!
    result->m_gameObject = static_pointer_cast<GameObject>(shared_from_this());

    SceneManager::data[scene()->id].new_components.emplace_back(
            static_pointer_cast<Component>(shared_from_this()));

    if constexpr (has_fieldAwake_v<T>) {
        auto pointer = result.get();
        auto f = [pointer]() {
            static_cast<T *>(pointer)->Awake();
        };

        if (Application::isPlaying() && activeInHierarchy()) {
            f();
        } else {
            to_awake.emplace(result, f);
        }
    }

    return static_pointer_cast<T>(result);
}
