
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

template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
TPtr<T> GameObject::AddComponent() {
    return AddComponent(std::make_shared<T>());
}
