#include "GameApi/BasicString.h"
#include "ThreadId.h"
#include <cstdarg>
#include <mutex>
#include <map>
#include <memory>
#include <cstdio>
#include <cinttypes>

namespace GameApi {

    size_t vsformat(const char *fmt, va_list ap, std::string &output) {
        //static thread_local std::vector<char> buf(64);
        static std::mutex m;
        static std::map<int, std::vector<char>> map;
        std::vector<char> *b2;
        {
            std::unique_lock l(m);
            b2 = &map[getCurrentThreadId()];
            if (b2->empty()) {
                b2->resize(64);
            }
        }
        std::vector<char> &buf = *b2;

        va_list aq;
        va_copy(aq, ap);
        size_t length = vsnprintf(buf.data(), buf.size(), fmt, aq);
        va_end(aq);

        if (length >= buf.size()) {
            size_t targetSize = buf.size();
            if (targetSize < 64) {
                targetSize = 64;
            }
            while (length >= targetSize) {
                targetSize *= 2;
            }

            buf.resize(targetSize);

            vsnprintf(buf.data(), buf.size(), fmt, ap);
        }

        output.append(buf.data(), length);

        return length;
    }

    std::string delete_from_string_separated_by(std::string str, const char &c, const std::string &del) {
        std::size_t found = 0;

        found = str.find(del, found);
        while (found != std::string::npos) {

            if ((found != 0 && str[found - 1] == c) || found == 0) {
                if (found != (str.size() - 1) && str[found + 1] == c) {
                    str.erase(found, del.size() + 1);
                } else if (found == (str.size() - 1)) {
                    str.erase(found, del.size());
                }
            }

            found = str.find(del, found);
        }

        return str;
    }

    std::string to_string(uint32_t number) {
        char str[12] = {0};
        std::sprintf(str, "%u", number);
        return str;
    }

    std::string to_string(int32_t number) {
        char str[12] = {0};
        std::sprintf(str, "%i", number);
        return str;
    }

    std::string to_hex(uint64_t number) {
        char str[17] = {0};
        std::sprintf(str, "%016" PRIx64, number);
        return str;
    }

    std::string to_string(void *pointer) {
        char str[24] = {0};
        std::sprintf(str, "%p", pointer);
        return str;
    }

    std::string to_string(float number) {
        char str[50] = {0};
        std::sprintf(str, "%f", number);
        return str;
    }

    std::string to_string(double number) {
        char str[50] = {0};
        std::sprintf(str, "%f", number);
        return str;
    }

    std::string to_string(float number, uint32_t how_much) {
        std::string result = to_string(number);

        std::size_t found = result.find('.');

        if (found != std::string::npos) {
            if (how_much != 0) { how_much += 1; }
            result.erase(found + how_much);
        }

        return result;
    }

    int32_t to_int(const std::string &str) {
        return atoi(str.c_str());
    }

    void *to_pointer(const std::string &str) {
        void *p;
        sscanf(str.c_str(), "%p", &p);
        return p;
    }

    double to_float(const std::string &str) {
        return atof(str.c_str());
    }

    bool is_int(const std::string &str) {
        int32_t result;
        return static_cast<bool>(sscanf(str.c_str(), "%i", &result));
    }

    std::string readFullFile(std::string_view path) {
        std::string result;
        FILE *file = fopen(path.data(), "rb");
        if (!file) { return {}; }
        std::shared_ptr<FILE> guard{file, [](FILE *f) { fclose(f); }};

        fseek(file, 0, SEEK_END);
        result.resize(ftell(file));
        fseek(file, 0, SEEK_SET);

        fread(result.data(), 1, result.size(), file);
        return result;
    }

    bool saveFullFile(std::string_view path, std::string_view data) {
        FILE *file = fopen(path.data(), "wb");
        if (!file) { return false; }
        std::shared_ptr<FILE> guard{file, [](FILE *f) { fclose(f); }};

        if (fwrite(data.data(), 1, data.length(), file) != data.length()) {
            return false;
        }

        return true;
    }

}
