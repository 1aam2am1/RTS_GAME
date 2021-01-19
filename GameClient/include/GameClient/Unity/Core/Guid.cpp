//
// Created by Michal_Marszalek on 21.10.2020.
//

#include "Guid.h"
#include <GameApi/BasicString.h>
#include <chrono>
#include <atomic>
#include <random>

namespace Unity {

    GUID::operator std::string() const {
        if (first == 0 && second == 0) { return "0"; }

        return GameApi::to_hex(first) + GameApi::to_hex(second);
    }

    GUID &GUID::operator=(const std::string &str) {
        if (str.empty()) {
            first = 0;
            second = 0;
        } else {
            first = strtoull(str.substr(0, 16).data(), nullptr, 16);
            if (str.length() > 16) {
                second = strtoull(str.substr(16, 16).data(), nullptr, 16);
            } else {
                second = 0;
            }
        }

        return *this;
    }

    GUID GUID::NewGuid() {
        GUID g;
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        static std::atomic<uint32_t> i = 1;
        static std::default_random_engine generator(std::random_device{}());
        static std::uniform_int_distribution<uint32_t> distribution(0, std::numeric_limits<uint32_t>::max());

        g.first = static_cast<uint64_t>(std::hash<uint64_t>{}(t) ^ distribution(generator)) << 32u;
        g.first |= static_cast<uint32_t>(t + (++i));

        g.second = static_cast<uint64_t>(std::hash<uint64_t>{}(distribution(generator))) << 32u;
        g.second |= static_cast<uint32_t>(std::hash<uint32_t>{}(distribution(generator)));

        return g;
    }

}
