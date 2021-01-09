
template<typename T, typename Y>
constexpr bool operator==(const TPtr<T> &l, const TPtr<Y> &r) noexcept {
    return static_cast<const Object *>(l.get()) == static_cast<const Object *>(r.get());
}

template<class T, class U>
std::strong_ordering operator<=>(const TPtr<T> &lhs, const TPtr<U> &rhs) noexcept {
    return std::compare_three_way{}(lhs.get(), rhs.get());
}

template<class T>
bool operator==(const TPtr<T> &lhs, std::nullptr_t) noexcept {
    return !lhs;
}

template<class T>
std::strong_ordering operator<=>(const TPtr<T> &lhs, std::nullptr_t) noexcept {
    using pointer = typename TPtr<T>::element_type *;
    return std::compare_three_way()(lhs.get(), static_cast<pointer>(nullptr));
}

template<class T, class U>
TPtr<T> dynamic_pointer_cast(const TPtr<U> &r) noexcept {
    TPtr<T> result;

    result = std::dynamic_pointer_cast<T>(r.ptr);

    return result;
}

template<class T, class U>
TPtr<T> dynamic_pointer_cast(TPtr<U> &&r) noexcept {
    TPtr<T> result;

    result = std::dynamic_pointer_cast<T>(std::move(r.ptr));
    r = nullptr;

    return result;
}

template<class T, class U>
TPtr<T> static_pointer_cast(const TPtr<U> &r) noexcept {
    TPtr<T> result;

    result = std::static_pointer_cast<T>(r.ptr);

    return result;
}

template<class T, class U>
TPtr<T> static_pointer_cast(TPtr<U> &&r) noexcept {
    TPtr<T> result;

    result = std::static_pointer_cast<T>(std::move(r.ptr));
    r = nullptr;

    return result;
}
