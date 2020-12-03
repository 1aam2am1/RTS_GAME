
template<typename... Args>
TPtr<GameObject> newGameObject(std::string name) {
    auto i = newGameObject(name);

    using array = int[sizeof...(Args)];
    array{(i->AddComponent<Args>(), 1)...};

    return i;
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
    auto result = components.emplace_back(this, std::make_shared<T>());

    //TODO: !!! Awake of Component when game running !!!
    result->m_gameObject = shared_from_this();

    if constexpr (has_fieldAwake_v<T>) {
        auto pointer = result.get();
        auto f = [pointer]() {
            static_cast<T *>(pointer)->Awake();
        };

        if (Application::isPlaying && activeInHierarchy()) {
            f();
        } else {
            to_awake.emplace(result, f);
        }
    }

    return result;
}
