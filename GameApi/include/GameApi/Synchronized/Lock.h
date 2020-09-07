//
// Created by Michal_Marszalek on 17.08.2020.
//

#ifndef RTS_GAME_LOCK_H
#define RTS_GAME_LOCK_H

#include "BaseSynchronized.h"

namespace GameApi {
    template<typename B, template<typename> typename L>
    class Lock {
    public:
        Lock() = delete;

        explicit Lock(B *ptr) : base(ptr), lock(std::make_shared<L<decltype(B::m)>>(ptr->m)) {

        }

        auto operator*() {
            return &base->member;
        }

        auto operator*() const {
            return &base->member;
        }

        auto operator->() {
            return &base->member;
        }

        auto operator->() const {
            return &base->member;
        }

    private:
        B *base;
        std::shared_ptr<L<decltype(B::m)>> lock;
    };

}


#endif //RTS_GAME_LOCK_H
