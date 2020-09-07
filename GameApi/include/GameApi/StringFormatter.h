//
// Created by Michal_Marszalek on 10.08.2020.
//

#ifndef RTS_GAME_STRINGFORMATTER_H
#define RTS_GAME_STRINGFORMATTER_H

#include <string>
#include <cstdarg>
#include <atomic>
#include <memory>
#include <utility>
#include "BasicString.h"
#include "Externalsstream.h"

#if defined(__GNUC__)
#define ATTRIBUTE_FORMAT(i, j) __attribute__ ((format (printf, i, j)))
#else
#define ATTRIBUTE_FORMAT(i, j)
#endif

namespace GameApi {

    class CacheableOSStream {
        CacheableOSStream() = default;

        explicit CacheableOSStream(std::string &output);

    public:
        static std::shared_ptr<CacheableOSStream> acquire(std::string &output);

        ExternalOStringStream stream;

    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
    };

    template<typename ChildType>
    class StringFormatter {
    public:
        explicit StringFormatter(std::string &output) :
                output{&output} {

        }

        virtual ~StringFormatter() = default;

        StringFormatter(const StringFormatter &) = delete;

        StringFormatter &operator=(const StringFormatter &) = delete;

        template<typename T>
        ChildType &operator<<(const T &t) {
            if (this->should_format()) {
                this->ensure_stream_obj();
                this->stream->stream << t;
            }
            return this->childRef();
        }

        ChildType &operator<<(const char *t) {
            if (this->should_format()) {
                this->output->append(t);
            }
            return this->childRef();
        }

        ChildType &operator<<(const std::string &t) {
            if (this->should_format()) {
                this->output->append(t);
            }
            return this->childRef();
        }

        inline ChildType &childRef() {
            return *static_cast<ChildType *>(this);
        }

        // Printf-style formatting
        ChildType &fmt(const char *fmt, ...) ATTRIBUTE_FORMAT(2, 3) {
            if (this->should_format()) {
                va_list ap;
                va_start(ap, fmt);
                GameApi::vsformat(fmt, ap, *this->output);
                va_end(ap);
            }

            return this->childRef();
        }

        /**
        * Returns if formatting should actually occur.
        */
        [[nodiscard]] virtual bool should_format() const {
            return true;
        }


    private:
        inline void ensure_stream_obj() {
            if (this->stream == nullptr) {
                this->stream = CacheableOSStream::acquire(*this->output);
                this->stream->stream.clear();
            }
        }

        std::string *output;
        std::shared_ptr<CacheableOSStream> stream;
    };

    class Formatter final : public StringFormatter<Formatter> {
    };

    class FString final : public StringFormatter<FString> {
    public:
        FString() :
                StringFormatter<FString>{this->buffer} {}

        operator std::string &() {
            return this->buffer;
        }

        operator const std::string &() const {
            return this->buffer;
        }

        operator std::string() && {
            return std::move(this->buffer);
        }

        std::string buffer;
    };

}


#endif //RTS_GAME_STRINGFORMATTER_H
