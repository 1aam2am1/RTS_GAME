//
// Created by Michal_Marszalek on 12.10.2020.
//

#include "Emitter.h"

#define PRINT_INDENT(str, indent) \
    for(int i = 0; i < (indent); ++i){(str) += " ";}

const std::string &Emitter::message() const {
    return str;
}

const char *Emitter::c_str() const {
    return str.data();
}

std::size_t Emitter::size() const {
    return str.size();
}

bool Emitter::good() const {
    return error.empty();
}

const std::string &Emitter::GetLastError() const {
    return error;
}

Emitter &Emitter::operator<<(const Node &n) {
    switch (n.data->type) {
        case NodeType::Scalar: {
            str += escape(n.Scalar());
            break;
        }
        case NodeType::Map: {
            if (!str.empty() && n.data->style != EmitterStyle::Flow) {
                str += "\n";
            }
            switch (n.data->style) {
                case EmitterStyle::Default:
                case EmitterStyle::Block: { //k: v \n, k: v
                    if (n.size()) {
                        for (auto &d: n) {
                            PRINT_INDENT(str, indent);
                            ///kluczem powinien byc scalar or alias
                            *this << d.first;
                            str += ": ";
                            indent += 1;
                            *this << d.second;
                            indent -= 1;
                            str += "\n";
                        }
                        str.pop_back(); //remove last \n
                    }
                }
                    break;
                case EmitterStyle::Flow: { //{k: v, k: v}
                    if (n.size()) {
                        str += "{";
                        for (auto &d: n) {
                            ///kluczem powinien byc scalar or alias
                            *this << d.first;
                            str += ": ";
                            indent += 1;
                            *this << d.second;
                            indent -= 1;
                            str += ", ";
                        }
                        str.pop_back(); //remove last ' '
                        str.pop_back(); //remove last ,
                        str += "}";
                    }
                }
                    break;
            }
            break;
        }
        case NodeType::Sequence: {
            if (!str.empty() && n.data->style != EmitterStyle::Flow) {
                str += "\n";
            }
            switch (n.data->style) {
                case EmitterStyle::Default:
                case EmitterStyle::Block: { // - scalar\n - scalar\n
                    if (n.size()) {
                        for (auto &d: n) {
                            PRINT_INDENT(str, indent);
                            str += "- ";
                            indent += 2;
                            *this << d.second;
                            indent -= 2;
                            str += "\n";
                        }
                        str.pop_back(); //remove last \n
                    }
                }
                    break;
                case EmitterStyle::Flow: { //[key, key]
                    if (n.size()) {
                        str += "[";
                        for (auto &d: n) {
                            *this << d.second;
                            str += ", ";
                        }
                        str.pop_back(); //remove last ' '
                        str.pop_back(); //remove last ,
                        str += "]";
                    }
                }
                    break;
            }
            break;
        }

        case NodeType::Null:
            str += "";
            break;
    }

    return *this;
}

Emitter &Emitter::operator<<(Document d) {
    if (str.empty()) {
        ///YAML VERSION
        switch (d.type) {
            case Document::NOPE:
            case Document::JSON:
                break;
            case Document::YAML1_2:
                str += "%YAML 1.2\n";
        }
        ///TAGS
        if (d.type != Document::YAML1_2) { d.tags.clear(); }

        for (const auto &t: d.tags) {
            str += "%TAG !" + t.first + "! " + t.second + "\n";
        }
    }
    if (!str.empty()) {
        str += "---\n";
    }

    for (auto it = d.nodes.begin(); it != d.nodes.end();) {
        *this << *it;
        ++it;
        if (it != d.nodes.end()) {
            str += "---\n";
        }
    }

    return *this;
}

void Emitter::clear() {
    error.clear();
    str.clear();
    indent = 0;
}

std::string Emitter::escape(std::string_view s) {
    std::string result;
    bool esc = false;

    for (auto c:s) {
        switch (c) {
            case '\"':
            case '\\':
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
            case '[':
            case ']':
            case '{':
            case '}':
            case ',':
            case ' ':
                esc = true;
                break;
            default:
                if ('\x00' <= c && c <= '\x1f') {

                }
        }
    }

    if (!esc) { return std::string{s}; }

    result += "\"";
    for (auto c:s) {
        switch (c) {
            case '"':
                result += "\\\"";
                break;
            case '\\':
                result += "\\\\";
                break;
            case '\b':
                result += "\\b";
                break;
            case '\f':
                result += "\\f";
                break;
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    result += "\\u";
                    auto str = std::to_string(c);
                    int i = 4;
                    while (i - str.size()) {
                        result += "0";
                        --i;
                    }
                    result += str;
                } else {
                    result += c;
                }
        }
    }
    result += "\"";

    return result;
}
