#ifndef ARGV_OPTIONS_H
#define ARGV_OPTIONS_H

#include <SFML/System/Vector2.hpp>
#include <cstdint>


class Argv_options {
public:
    Argv_options() = default;

    virtual ~Argv_options() = default;

    bool process(int argc, char **argv);

    struct Options {
        sf::Vector2u size = {0, 0};
        bool version = false;
        int32_t fps = -1;
    };

    [[nodiscard]] const Options &getOptions() const;

    void setOptions(Options);

protected:

private:
    Options options;
};

#endif // ARGV_OPTIONS_H
