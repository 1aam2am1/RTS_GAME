//
// Created by Michal_Marszalek on 11.08.2020.
//

#ifndef RTS_GAME_EXTERNALSSTREAM_H
#define RTS_GAME_EXTERNALSSTREAM_H


#include <ostream>

namespace GameApi {
    class ExternalStringBuf : public std::streambuf {
    public:

        std::string *output = nullptr;
    protected:
        std::streamsize xsputn(const char_type *s, std::streamsize n) override;

        int overflow(int_type c) override;
    };

    class ExternalOStringStream : public std::ostream {
    public:
        explicit ExternalOStringStream() :
                std::ostream{&buf} {}

        inline void setOutput(std::string &output) {
            buf.output = &output;
        }

    private:
        ExternalStringBuf buf;
    };
}


#endif //RTS_GAME_EXTERNALSSTREAM_H
