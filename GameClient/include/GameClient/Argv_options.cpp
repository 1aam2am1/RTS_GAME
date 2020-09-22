#include "GameClient/Argv_options.h"
#include <getopt.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include "GameAPI/BasicString.h"
#include <GameApi/GlobalLogSource.h>

bool Argv_options::process(int argc, char **argv) {
    const option long_options[] =
            {
                    {"geometry", required_argument, nullptr, 'g'},
                    {"help",     no_argument,       nullptr, 'h'},
                    {"version",  no_argument,       nullptr, 'v'},
                    {"fps",      required_argument, nullptr, 'f'},
                    {nullptr,    0,                 nullptr, 0}
            };
    int options_long_string_length = 0;

    optind = 0; //set to 0 because of multiple processing

    std::string getopt_long_short_options;
    getopt_long_short_options.reserve((sizeof(long_options) / sizeof(option)) * 2);

    for (uint32_t i = 0; i < ((sizeof(long_options) / sizeof(option)) - 1); ++i) {
        getopt_long_short_options.push_back(static_cast<char>(long_options[i].val));
        if (long_options[i].has_arg != no_argument) {
            getopt_long_short_options.push_back(':');
        }

        options_long_string_length = std::max<int>(options_long_string_length, strlen(long_options[i].name));
    }

    while (true) {
        int option_index = 0;

        int c = getopt_long(argc, argv, getopt_long_short_options.c_str(),
                            long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
            case 'g': {
                std::string str = optarg;
                if (str.find('x') != std::string::npos) {
                    unsigned int x = GameApi::to_int(GameApi::substr(str, 'x'));
                    unsigned int y = GameApi::to_int(str);
                    if (x == 0) { x = options.size.x; }
                    if (y == 0) { y = options.size.x; }
                    options.size = {x, y};
                } else if (str.find('X') != std::string::npos) {
                    unsigned int x = GameApi::to_int(GameApi::substr(str, 'X'));
                    unsigned int y = GameApi::to_int(str);
                    if (x == 0) { x = options.size.x; }
                    if (y == 0) { y = options.size.x; }
                    options.size = {x, y};
                } else {
                    GameApi::log(INFO << "Option -g as --geometry required widthXheight\n");
                }
            }
                break;
            case 'v':
                options.version = true;
                break;
            case 'f': {
                std::string str = optarg;
                options.fps = GameApi::to_int(str);
            }
                break;
            case 'h': {
                auto message = INFO;
                message << "Options:\n";
                for (uint32_t i = 0; long_options[i].name != nullptr || long_options[i].has_arg != 0 ||
                                     long_options[i].flag != nullptr || long_options[i].val != 0; ++i) {
                    message.fmt(" - %-*s %s\n", options_long_string_length + 2,
                                long_options[i].name,
                                long_options[i].has_arg ? "Arg" : "");
                }
                GameApi::log(message);
                exit(EXIT_FAILURE);
            }
            case '?':
                GameApi::log(ERR << "ERROR Bad Option\n");
                exit(EXIT_FAILURE);
            default:
                GameApi::log(ERR << "ERROR Bad Option\n");
                return false;
        }
    }

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        auto message = ERR;
        message << "non-option ARGV-elements: ";
        while (optind < argc)
            message.fmt("%s ", argv[optind++]);
        GameApi::log(message);
    }

    return true;
}

const Argv_options::Options &Argv_options::getOptions() const {
    return options;
}

void Argv_options::setOptions(Argv_options::Options op) {
    this->options = op;
}
