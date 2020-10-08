#pragma once

#include <string_view>

struct Version {
    static const std::string_view GIT_DIRTY;
    static const std::string_view GIT_SHA;
    static const std::string_view GIT_TAG;
    static const std::string_view DATE;
};
