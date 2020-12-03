template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
TPtr<T> Component::GetComponent() const {
    return m_gameObject->GetComponent<T>();
}

template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
TPtr<T> Component::GetComponentInChildren() const {
    return m_gameObject->GetComponentInChildren<T>();
}

template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
TPtr<T> Component::GetComponentInParent() const {
    return m_gameObject->GetComponentInParent<T>();
}

template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
std::vector<TPtr<T>> Component::GetComponents() const {
    return m_gameObject->GetComponents<T>();
}

template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
std::vector<TPtr<T>> Component::GetComponentsInChildren(bool includeInactive) const {
    return m_gameObject->GetComponentsInChildren<T>(includeInactive);
}

template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
std::vector<TPtr<T>> Component::GetComponentsInParent(bool includeInactive) const {
    return m_gameObject->GetComponentsInParent<T>(includeInactive);
}
