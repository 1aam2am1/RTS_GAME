//
// Created by Michal_Marszalek on 11.08.2020.
//

#ifndef RTS_GAME_LOGLEVEL_H
#define RTS_GAME_LOGLEVEL_H

#include <string>
#include <typeinfo>
#include <iostream>

namespace GameApi {

    class level_value {
    public:
        constexpr level_value(const char *n, int v, const char *c) : name(n), value(v), colorCode(c) {}

        level_value(const level_value &other) = delete;

        level_value &operator=(const level_value &other) = delete;

        constexpr bool operator==(const level_value &other) const {
            return (this == &other);
        }

        constexpr bool operator!=(const level_value &other) const {
            return !(*this == other);
        }

        constexpr bool operator<=(const level_value &other) const {
            return this->value <= other.value;
        }

        constexpr bool operator<(const level_value &other) const {
            return this->value < other.value;
        }

        constexpr bool operator>=(const level_value &other) const {
            return this->value >= other.value;
        }

        constexpr bool operator>(const level_value &other) const {
            return this->value > other.value;
        }

        friend std::ostream &operator<<(std::ostream &os, const level_value &arg) {
            os << typeid(level_value).name() << "::" << arg.name;
            return os;
        }

        const char *name;
        int value;
        const char *colorCode;
    };

    struct logLevel {
        logLevel();

        constexpr logLevel(const level_value &levelValue) : lv{&levelValue} {
        }

        constexpr logLevel &operator=(const level_value &v) {
            this->lv = &v;
            return *this;
        }

        constexpr bool operator>=(const logLevel &v) const {
            return (*this)->value >= v->value;
        }

        static constexpr level_value MIN{"min loglevel", -1000, "5"};

        static constexpr level_value spam{"SPAM", -100, ""};
        static constexpr level_value dbg{"DBG", -20, ""};
        static constexpr level_value info{"INFO", 0, ""};
        static constexpr level_value warn{"WARN", 100, "33"};
        static constexpr level_value err{"ERR", 200, "31;1"};
        static constexpr level_value crit{"CRIT", 500, "31;1;47"};

        static constexpr level_value MAX{"max loglevel", 1000, "5"};


        constexpr const level_value *operator->() const {
            return lv;
        }

        const level_value *lv;
    };

}

#endif //RTS_GAME_LOGLEVEL_H
