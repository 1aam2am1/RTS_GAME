//
// Created by Michal_Marszalek on 16.08.2020.
//

#ifndef RTS_GAME_SYNCHRONIZED_H
#define RTS_GAME_SYNCHRONIZED_H

#include <shared_mutex>
#include "BaseSynchronized.h"
#include "Lock.h"

namespace GameApi {
    template<typename T, typename M = std::shared_mutex>
    class Synchronized : private BaseSynchronized<T, M> {
        using Base = BaseSynchronized<T, M>;
    public:
        using WLockPtr = Lock<Base, std::unique_lock>;
        using RLockPtr = Lock<Base, std::shared_lock>;

        template<typename... Args>
        explicit Synchronized(Args &&... args) :
                Base(std::forward<Args>(args)...) {

        }

        [[nodiscard]] WLockPtr wLock() {
            return WLockPtr(static_cast<Base *>(this));
        }

        [[nodiscard]] RLockPtr rLock() const {
            return RLockPtr(static_cast<const Base *>(this));
        }

        template<typename Function>
        auto withWLock(Function &&f) {
            return f(*wLock());
        }

        template<typename Function>
        auto withRLock(Function &&f) const {
            return f(*rLock());
        }

        friend WLockPtr;
        friend RLockPtr;
    };

}

#endif //RTS_GAME_SYNCHRONIZED_H
