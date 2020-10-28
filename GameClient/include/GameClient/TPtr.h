//
// Created by Michal_Marszalek on 08.09.2020.
//

#ifndef RTS_GAME_TPTR_H
#define RTS_GAME_TPTR_H

#include <memory>
#include <Core/Object.h>
#include <GameApi/IsInstance.h>


template<typename T>
class TPtr {
public:

    /// Don't create default pointer without parent
    TPtr() = delete;

    /// Create copy but, when return from function but watch out for parent as it could be destroyed.
    TPtr(const TPtr &r) {
        *this = r;
    }

    /// Don't move when parent can't be moved
    TPtr(TPtr &&r) = delete;

    /// Create new object
    /// \tparam Y type of object
    /// \param parent parent of this object that is Object as we need to reset shared when our object is destroyed
    /// \param ptr object
    template<typename Y = T>
    constexpr explicit TPtr(Object *parent, const std::shared_ptr<Y> &ptr = nullptr) noexcept : ptr(ptr) {
        if (parent) { parentConnection = parent->onDestroySignal.connect_scoped(&TPtr::destroy, this); }
    }

    ~TPtr() {
        static_assert(std::is_base_of_v<Object, T>, "only subclasses, please");

        if (ptr) { ptr->onDestroySignal.disconnect(&TPtr::destroy, this); }
        parentConnection.disconnect();
    }

    /// Copy shared_ptr of object
    /// \param r To copy
    /// \return *this
    template<class U>
    TPtr &operator=(U &&r) noexcept {
        if (ptr) {
            ptr->onDestroySignal.disconnect(&TPtr::destroy, this);
        }

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

        if (ptr) {
            ptr->onDestroySignal.connect(&TPtr::destroy, this);
        }

        return *this;
    }

    TPtr &operator=(const TPtr &r) noexcept {
        if (ptr) {
            ptr->onDestroySignal.disconnect(&TPtr::destroy, this);
        }

        ptr = r.ptr;

        if (ptr) {
            ptr->onDestroySignal.connect(&TPtr::destroy, this);
        }

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

    T *get() const noexcept {
        return ptr.get();
    }

private:
    std::shared_ptr<T> ptr;
    sigslot::scoped_connection parentConnection;

    constexpr void destroy() noexcept {
        ptr.reset();
    }

    template<typename>
    friend
    class TPtr;

    template<class X, class U>
    friend TPtr<X> dynamic_pointer_cast(const TPtr<U> &) noexcept;
};

template<class T, class U>
TPtr<T> dynamic_pointer_cast(const TPtr<U> &r) noexcept {
    TPtr<T> result{nullptr};

    result.ptr = std::dynamic_pointer_cast<T>(r.ptr);

    if (result.ptr) {
        result.ptr->onDestroySignal.connect(&TPtr<T>::destroy, &result);
    }

    return result;
}

#include <GameClient/Unity/Macro.h>

#endif //RTS_GAME_TPTR_H
