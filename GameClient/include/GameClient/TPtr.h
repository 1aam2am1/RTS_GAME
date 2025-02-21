//
// Created by Michal_Marszalek on 08.09.2020.
//

#ifndef RTS_GAME_TPTR_H
#define RTS_GAME_TPTR_H

#include <memory>
#include <GameApi/IsInstance.h>
#include <GameApi/Signal.h>

class Object;

template<typename T = Object>
class TPtr {
public:
    using element_type = std::remove_extent_t<T>;

    constexpr TPtr() noexcept = default;

    constexpr TPtr(std::nullptr_t) noexcept {}

    TPtr(const TPtr &r) {
        *this = r;
    }

    template<typename Y>
    TPtr(const TPtr<Y> &r) {
        *this = dynamic_pointer_cast<T>(r);
    }

    TPtr(TPtr &&r) noexcept {
        *this = std::move(r);
    }

    template<typename Y>
    TPtr(TPtr<Y> &&r) noexcept {
        *this = dynamic_pointer_cast<T>(std::move(r));
    }

    template<typename Y>
    TPtr(const std::shared_ptr<Y> &r) noexcept: ptr(std::dynamic_pointer_cast<T>(r)) {
        if (ptr) { objectConnection = ptr->onDestroySignal.connect(&TPtr::destroy, this); }
    }

    template<typename Y>
    TPtr(std::shared_ptr<Y> &&r) noexcept: ptr(std::dynamic_pointer_cast<T>(std::move(r))) {
        if (ptr) { objectConnection = ptr->onDestroySignal.connect(&TPtr::destroy, this); }
    }

    ~TPtr() {
        static_assert(std::is_base_of_v<Object, T>, "only subclasses, please");
        objectConnection.disconnect();
    }

    template<class U>
    TPtr &operator=(U &&r) noexcept {
        objectConnection.disconnect();

        if constexpr (is_instance_v<U, TPtr>) {
            ptr = std::forward<U>(r).ptr;
        } else if constexpr (is_instance_v<U, std::shared_ptr>) {
            ptr = std::forward<U>(r);
        } else if constexpr (std::is_same_v<U, std::nullptr_t>) {
            ptr = nullptr;
            return *this;
        } else {
            static_assert(is_instance_v<U, TPtr>, "Only shared, TPtr or nullptr");
        }

        //if moved r.ptr == nullptr, therefore delete moved ptr
        if constexpr(std::is_rvalue_reference_v<U &&>) {
            if constexpr (is_instance_v<U, TPtr>) {
                r.objectConnection.disconnect();
                r.ptr = nullptr;
            } else if constexpr (is_instance_v<U, std::shared_ptr>) {
                r = nullptr;
            }
        }

        if (ptr) {
            objectConnection = ptr->onDestroySignal.connect(&TPtr::destroy, this);
        }

        on_change(*this);
        return *this;
    }

    TPtr &operator=(const TPtr &r) noexcept {
        objectConnection.disconnect();

        ptr = r.ptr;

        if (ptr) {
            objectConnection = ptr->onDestroySignal.connect(&TPtr::destroy, this);
        }

        on_change(*this);
        return *this;
    }

    TPtr &operator=(TPtr &&r) noexcept {
        objectConnection.disconnect();

        ptr = std::move(r.ptr);
        r.objectConnection.disconnect();
        r.ptr = nullptr;

        if (ptr) {
            objectConnection = ptr->onDestroySignal.connect(&TPtr::destroy, this);
        }

        on_change(*this);
        return *this;
    }

    [[nodiscard]] bool expired() const noexcept { return ptr.get() == nullptr; }

    explicit operator bool() const noexcept {
        return ptr.operator bool();
    }

    T *operator->() const {
        if (*this) {
            return ptr.get();
        } else {
            throw std::bad_weak_ptr();
        }
    }

    T &operator*() const {
        if (*this) {
            return *ptr.get();
        } else {
            throw std::bad_weak_ptr();
        }
    }

    [[nodiscard]] T *get() const noexcept {
        return ptr.get();
    }

    ///Reset all object to new one
    void reset(TPtr<Object> o) {
        if (ptr && ptr != o.ptr) {
            ptr->onDestroySignal(o.get());
        } else {
            *this = dynamic_pointer_cast<T>(o);
        }
    }

    [[nodiscard]] long use_count() const noexcept {
        return ptr.use_count();
    }

    /// Emits signal when pointer changed
    sigslot::signal<TPtr<T> &> on_change{};
private:
    std::shared_ptr<T> ptr;
    sigslot::scoped_connection objectConnection;

    void destroy(Object *o) noexcept {
        T *t = dynamic_cast<T *>(o);
        if (t) {
            *this = static_pointer_cast<T>(t->shared_from_this());
        } else {
            *this = nullptr;
            on_change(*this);
        }
    }

    template<typename>
    friend
    class TPtr;

    template<class X, class U>
    friend TPtr<X> dynamic_pointer_cast(const TPtr<U> &) noexcept;

    template<class X, class U>
    friend TPtr<X> dynamic_pointer_cast(TPtr<U> &&) noexcept;

    template<class X, class U>
    friend TPtr<X> static_pointer_cast(const TPtr<U> &) noexcept;

    template<class X, class U>
    friend TPtr<X> static_pointer_cast(TPtr<U> &&) noexcept;

    template<typename>
    friend
    class std::hash;

    friend
    class MetaData;
};

template<typename T, typename Y>
constexpr bool operator==(const TPtr<T> &l, const TPtr<Y> &r) noexcept;

template<class T, class U>
std::strong_ordering operator<=>(const TPtr<T> &lhs, const TPtr<U> &rhs) noexcept;

template<class T>
bool operator==(const TPtr<T> &lhs, std::nullptr_t) noexcept;

template<class T>
std::strong_ordering operator<=>(const TPtr<T> &lhs, std::nullptr_t) noexcept;

template<class T, class U>
TPtr<T> dynamic_pointer_cast(const TPtr<U> &r) noexcept;

template<class T, class U>
TPtr<T> dynamic_pointer_cast(TPtr<U> &&r) noexcept;

template<class T, class U>
TPtr<T> static_pointer_cast(const TPtr<U> &r) noexcept;

template<class T, class U>
TPtr<T> static_pointer_cast(TPtr<U> &&r) noexcept;

#include "TPtr.inl"

namespace std {
    template<typename T>
    struct hash<TPtr<T>> {
        std::size_t operator()(const TPtr<T> &t) const {
            return std::hash<decltype(t.ptr)>()(t.ptr);
        }
    };
}

#endif //RTS_GAME_TPTR_H
