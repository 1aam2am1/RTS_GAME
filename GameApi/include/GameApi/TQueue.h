//
// Created by Michal_Marszalek on 15.08.2020.
//

#ifndef RTS_GAME_TQUEUE_H
#define RTS_GAME_TQUEUE_H

#include <queue>
#include <mutex>

namespace GameApi {

    template<typename T>
    class TQueue {
    public:
        bool pollEvent(T &x) {
            std::unique_lock l(m);
            if (u.empty()) {
                return false;
            }

            x = std::move(u.front());
            u.pop();
            return true;
        }

        void push_back(const T &x) {
            std::unique_lock l(m);
            u.push(x);
        }

        void push_back(T &&x) {
            std::unique_lock l(m);
            u.push(std::move(x));
        }

        [[nodiscard]] bool empty() const {
            std::unique_lock l(m);
            return u.empty();
        }

        explicit operator bool() const {
            return empty();
        }

    private:
        std::mutex m;
        std::queue<T> u;
    };

}

#endif //RTS_GAME_TQUEUE_H
