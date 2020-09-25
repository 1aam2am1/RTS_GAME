//
// Created by Michal_Marszalek on 25.09.2020.
//

#include "Compiler.h"

#ifndef _WIN32
#include <cxxabi.h>
#include <dlfcn.h>
#endif

std::string GameApi::demangle(std::string_view symbol) {
#ifdef _WIN32
    // MSVC's typeid(T).name() already returns a demangled name.
    return std::string{symbol};
#else
    int status;
    char *buf = abi::__cxa_demangle(symbol, nullptr, nullptr, &status);

    if (status != 0) {
        return symbol;
    } else {
        std::string result{buf};
        free(buf);
        return result;
    }
#endif
}
