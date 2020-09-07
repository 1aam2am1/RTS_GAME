//
// Created by Michal_Marszalek on 17.08.2020.
//

#ifndef RTS_GAME_BASESYNCHRONIZED_H
#define RTS_GAME_BASESYNCHRONIZED_H

#include <memory>

namespace GameApi {

    template<typename, template<typename> typename>
    class Lock;

    template<typename T, typename M>
    class BaseSynchronized {
    public:
        BaseSynchronized() = delete;

        template<typename... Args>
        explicit BaseSynchronized(Args &&... args) :
                m{}, member{std::forward<Args>(args)...} {

        }


    protected:
        mutable M m;
        T member;

        template<typename, template<typename> typename>
        friend
        class Lock;
    };

}

#endif //RTS_GAME_BASESYNCHRONIZED_H
