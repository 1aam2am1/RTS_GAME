#ifndef STRING_H
#define STRING_H

#ifdef _WIN32

#include <io.h>
#include <direct.h>

#endif // _WIN32
#ifdef __linux__
#include <sys/dir.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif // __linux__

#include <string>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

namespace GameApi {

    inline std::string substr(std::string &str, const char &c = ' ') {
        std::string result;

        std::size_t found = str.find(c); //first character counting from 0

        if (found != std::string::npos) {
            result.insert(0, str, 0, found); //places str from 0 to found (counting from 1)
            str.erase(0, found + 1);
        } else {
            result = std::move(str);
            str.clear();
        }

        return result;
    }

    inline std::string rsubstr(std::string &str, const char &c = ' ') {
        std::string result;

        std::size_t found = str.rfind(c); //first character counting from 0

        if (found != std::string::npos) {
            result.insert(0, str, found + 1,
                          str.length() - found); //places str from found (counting from 1) to end
            str.erase(found, str.length() - found);
        } else {
            result = std::move(str);
            str.clear();
        }

        return result;
    }

    inline std::string csubstr(std::string str, const char &c = ' ') {
        return substr(str, c);
    }

    inline std::string crsubstr(std::string str, const char &c = ' ') {
        return rsubstr(str, c);
    }

    size_t vsformat(const char *fmt, va_list ap, std::string &output);

    std::string delete_from_string_separated_by(std::string str, const char &c = ' ', const std::string &del = ".");

    std::string to_string(uint32_t number);

    std::string to_string(int32_t number);

    std::string to_string(void *pointer);

    std::string to_string(float number);

    std::string to_string(double number);

    std::string to_string(float number, uint32_t how_much);

    int32_t to_int(const std::string &str);

    void *to_pointer(const std::string &str);

    double to_float(const std::string &str);

    bool is_int(const std::string &str);

    inline std::string to_width_string(std::string s, int w) {
        s.insert(s.end(), w - s.size(), ' ');
        return s;
    }

    inline bool hasPrefix(const std::string &s, const std::string_view p) {
        for (uint32_t i = 0; i < p.size(); ++i) {
            if (s[i] != p[i]) {
                if (!((s[i] == '\\' || p[i] == '\\') && (s[i] == '/' || p[i] == '/')))
                    return false;
            }
        }
        return true;
    }

    inline std::string removePrefix(std::string s, const std::string_view p) {
        if (hasPrefix(s, p)) {
            s.erase(0, p.size());
        }
        return s;
    }

    namespace files {
        //std::string dir(std::string gdzie);
        std::vector<std::string> dirD(std::string folder);

        std::vector<std::string> dirF(std::string folder);

        bool mkdir(const std::string &name);
    }

    namespace binary {
        template<typename T>
        inline T &shift_bit_left(T &co, const int8_t &ile) {
            static_assert(std::is_unsigned<T>{}, "T should be unsigned");
            asm volatile( "rol %%cl, %0" : "+r"( co ) : "ecx"( ile ));
            return co;
        }

        template<typename T>
        inline T &shift_bit_right(T &co, const int8_t &ile) {
            return shift_bit_left(co, -ile);
        }
    }


    template<typename Tag, typename Tag::type Member>
    struct break_in_ {
        friend constexpr typename Tag::type break_in(Tag) {
            return Member;
        }
    };

#define DEFINE_BREAK_IN(name, t, member)\
           struct name {\
             using type = t;\
             friend constexpr t break_in(name);\
           };\
           template struct break_in_<name, member>

    //DEFINE_BREAK_IN(nazwa_odwolamnia, int Klasa::*, &Klasa::x);
    // std::cout << x. *break_in(nazwa_odwolamnia());
}
#endif // STRING_H
