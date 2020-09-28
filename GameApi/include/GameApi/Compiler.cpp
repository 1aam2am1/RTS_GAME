//
// Created by Michal_Marszalek on 25.09.2020.
//

#include "Compiler.h"
#include <cxxabi.h>

std::string GameApi::demangle(std::string_view symbol) {
    int status;
    char *buf = abi::__cxa_demangle(symbol.data(), nullptr, nullptr, &status);

    if (status != 0) {
        return std::string{symbol};
    } else {
        std::string result{buf};
        free(buf);
        return result;
    }
}
