//
// Created by Michal_Marszalek on 11.08.2020.
//

#include "Externalsstream.h"

std::streamsize GameApi::ExternalStringBuf::xsputn(const char *s, std::streamsize n) {
    if (output != nullptr) {
        output->append(s, n);
        return n;
    }

    return 0;
}

int GameApi::ExternalStringBuf::overflow(int c) {
    if (output != nullptr) {
        output->push_back(c);
        return c;
    }

    return std::streambuf::traits_type::eof();
}
