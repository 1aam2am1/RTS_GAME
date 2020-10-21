#include "GameApi/BasicString.h"
#include "ThreadId.h"
#include <cstdarg>
#include <mutex>
#include <map>

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
        char str[12] = {0};
        std::sprintf(str, "%016llx", number);
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

    namespace files {
#ifdef _WIN32

        std::vector<std::string> dirD(std::string folder) {
            _finddata_t data{};
            std::vector<std::string> result;

            if (folder.back() != '/') { folder += "/"; }
            if (folder.back() == '/') { folder += "*"; }

            intptr_t handle = _findfirst(folder.c_str(), &data);
            if (handle == -1) return result;

            if (data.attrib & _A_SUBDIR) { result.emplace_back(data.name); }
            int32_t find = _findnext(handle, &data);
            while (find != -1) {
                if (data.attrib & _A_SUBDIR) {
                    result.emplace_back(data.name);
                }
                find = _findnext(handle, &data);
            }
            _findclose(handle);
            return result;
        }

        std::vector<std::string> dirF(std::string folder) {
            _finddata_t data{};
            std::vector<std::string> result;

            if (folder.back() != '/') { folder += "/"; }
            if (folder.back() == '/') { folder += "*"; }

            intptr_t handle = _findfirst(folder.c_str(), &data);
            if (handle == -1) return result;

            if (!(data.attrib & _A_SUBDIR)) { result.emplace_back(data.name); }
            int32_t find = _findnext(handle, &data);
            while (find != -1) {
                if (!(data.attrib & _A_SUBDIR)) {
                    result.emplace_back(data.name);
                }
                find = _findnext(handle, &data);
            }
            _findclose(handle);
            return result;
        }

        bool mkdir(const std::string &d) {
            if (::mkdir(d.c_str())) {
                switch (errno) {
                    case EEXIST:
                        return true;
                    case ENOENT:
                    default:
                        return false;
                }
            }
            return true;
        }

#endif // _WIN32
#ifdef __linux__

        std::vector<std::string> dirD(std::string gdzie)
        {
            DIR *dir;
            dirent *drnt;
            std::vector<std::string> result;

            if(gdzie.empty()){gdzie = ".";}

            if( (dir = opendir(gdzie.c_str())) != nullptr) //otwieram folder
            {
                drnt = readdir( dir ); //czytam 1

                while(drnt != nullptr)
                {
                    if(drnt->d_type == DT_DIR){result.emplace_back(drnt->d_name);} //czytam nazwe
                    drnt = readdir( dir );
                }

                closedir(dir); //zamykam folder
            }

            return result;
        }

        std::vector<std::string> dirF(std::string gdzie) {
            DIR *dir;
            dirent *drnt;
            std::vector<std::string> result;

            if (gdzie.empty()) { gdzie = "."; }

            if ((dir = opendir(gdzie.c_str())) != nullptr) //otwieram folder
            {
                drnt = readdir(dir); //czytam 1

                while (drnt != nullptr) {
                    if (drnt->d_type != DT_DIR) { result.emplace_back(drnt->d_name); } //czytam nazwe
                    drnt = readdir(dir);
                }

                closedir(dir); //zamykam folder
            }

            return result;
        }

        bool mkdir(std::string jaki) {
            if (::mkdir(jaki.c_str(), 770)) {
                switch (errno) {
                    case EEXIST:
                        return true;
                    case ENOENT:
                        return false;
                    default:
                        return false;
                }
            }
            return true;
        }

#endif // __linux__
    }


}
