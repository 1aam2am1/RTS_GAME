//
// Created by Michal_Marszalek on 08.09.2020.
//

#ifndef RTS_GAME_TPTR_H
#define RTS_GAME_TPTR_H

#include <memory>
#include <Core/Object.h>


template<typename T>
class TPtr {
public:

    /// Don't create default pointer without parent
    TPtr() = delete;

    /// Don't crate copy when parent can't be copied
    TPtr(const TPtr &r) = delete;

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
    template<class Y = T>
    TPtr &operator=(const TPtr<Y> &r) noexcept {
        if (ptr) {
            ptr->onDestroySignal.disconnect(&TPtr::destroy, this);
        }

        ptr = r.ptr;

        if (ptr) {
            ptr->onDestroySignal.connect(&TPtr::destroy, this);
        }

        return *this;
    }

    /// Copy shared_ptr of object
    /// \param r To copy
    /// \return *this
    template<class Y = T>
    TPtr &operator=(const std::shared_ptr<Y> &r) noexcept {
        static_assert(std::is_base_of_v<T, Y>, "only subclasses, please");
        if (ptr) {
            ptr->onDestroySignal.disconnect(&TPtr::destroy, this);
        }

        ptr = r.ptr;

        if (ptr) {
            ptr->onDestroySignal.connect(&TPtr::destroy, this);
        }

        return *this;
    }

    TPtr &operator=(std::nullptr_t) noexcept {
        if (ptr) {
            ptr->onDestroySignal.disconnect(&TPtr::destroy, this);
        }

        ptr = nullptr;

        return *this;
    }

    /// Move shared_ptr of object
    /// \param r To move
    /// \return *this
    template<class Y = T>
    TPtr &operator=(TPtr<Y> &&r) noexcept {
        if (ptr) {
            ptr->onDestroySignal.disconnect(&TPtr::destroy, this);
        }

        ptr = std::move(r.ptr);

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

};

#include <GameClient/Unity/Macro.h>

#endif //RTS_GAME_TPTR_H
