//
// Created by Michal_Marszalek on 10.10.2020.
//

#include "convert.h"

bool IsUpper(char ch) { return 'A' <= ch && ch <= 'Z'; }

char ToLower(char ch) { return IsUpper(ch) ? ch + 'a' - 'A' : ch; }

std::string tolower(const std::string &str) {
    std::string s(str);
    std::transform(s.begin(), s.end(), s.begin(), ToLower);
    return s;
}

bool convert<bool>::decode(const Node &node, bool &rhs) {
    if (!node.IsScalar())
        return false;

    // we can't use iostream bool extraction operators as they don't
    // recognize all possible values in the table below (taken from
    // http://yaml.org/type/bool.html)
    static const struct {
        std::string truename, falsename;
    } names[] = {
            {"y",    "n"},
            {"yes",  "no"},
            {"true", "false"},
            {"on",   "off"},
    };


    for (const auto &name : names) {
        if (name.truename == tolower(node.Scalar())) {
            rhs = true;
            return true;
        }

        if (name.falsename == tolower(node.Scalar())) {
            rhs = false;
            return true;
        }
    }

    return false;
}
