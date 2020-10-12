#ifndef RTS_GAME_CONVERT_H
#define RTS_GAME_CONVERT_H

#include <array>
#include <cmath>
#include <limits>
#include <list>
#include <map>
#include <sstream>
#include <type_traits>
#include <vector>

#include <Yaml/Node/Node.h>


template<typename T>
struct convert;

inline bool IsInfinity(const std::string &input) {
    return input == ".inf" || input == ".Inf" || input == ".INF" ||
           input == "+.inf" || input == "+.Inf" || input == "+.INF";
}

inline bool IsNegativeInfinity(const std::string &input) {
    return input == "-.inf" || input == "-.Inf" || input == "-.INF";
}

inline bool IsNaN(const std::string &input) {
    return input == ".nan" || input == ".NaN" || input == ".NAN";
}


// Node
template<>
struct convert<Node> {
    static Node encode(const Node &rhs) { return rhs; }

    static bool decode(const Node &node, Node &rhs) {
        rhs.reset(node);
        return true;
    }
};

// std::string
template<>
struct convert<std::string> {
    static Node encode(const std::string &rhs) { return Node(rhs); }

    static bool decode(const Node &node, std::string &rhs) {
        if (!node.IsScalar())
            return false;
        rhs = node.Scalar();
        return true;
    }
};

template<>
struct convert<std::nullptr_t> {
    static Node encode(std::nullptr_t) { return Node(); }

    static bool decode(const Node &node, std::nullptr_t) {
        return node.IsNull();
    }
};

namespace conversion {
    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value, void>::type
    inner_encode(const T &rhs, std::stringstream &stream) {
        if (std::isnan(rhs)) {
            stream << ".nan";
        } else if (std::isinf(rhs)) {
            if (std::signbit(rhs)) {
                stream << "-.inf";
            } else {
                stream << ".inf";
            }
        } else {
            stream << rhs;
        }
    }

    template<typename T>
    typename std::enable_if<!std::is_floating_point<T>::value, void>::type
    inner_encode(const T &rhs, std::stringstream &stream) {
        stream << rhs;
    }

    template<typename T>
    typename std::enable_if<(std::is_same<T, unsigned char>::value ||
                             std::is_same<T, signed char>::value), bool>::type
    ConvertStreamTo(std::stringstream &stream, T &rhs) {
        int num;
        if ((stream >> std::noskipws >> num) && (stream >> std::ws).eof()) {
            if (num >= (std::numeric_limits<T>::min)() &&
                num <= (std::numeric_limits<T>::max)()) {
                rhs = num;
                return true;
            }
        }
        return false;
    }

    template<typename T>
    typename std::enable_if<!(std::is_same<T, unsigned char>::value ||
                              std::is_same<T, signed char>::value), bool>::type
    ConvertStreamTo(std::stringstream &stream, T &rhs) {
        if ((stream >> std::noskipws >> rhs) && (stream >> std::ws).eof()) {
            return true;
        }
        return false;
    }
}

#define YAML_DEFINE_CONVERT_STREAMABLE(type, negative_op)                  \
  template <>                                                              \
  struct convert<type> {                                                   \
                                                                           \
    static Node encode(const type& rhs) {                                  \
      std::stringstream stream;                                            \
      stream.precision(std::numeric_limits<type>::max_digits10);           \
      conversion::inner_encode(rhs, stream);                               \
      return Node(stream.str());                                           \
    }                                                                      \
                                                                           \
    static bool decode(const Node& node, type& rhs) {                      \
      if (node.Type() != NodeType::Scalar) {                               \
        return false;                                                      \
      }                                                                    \
      const std::string& input = node.Scalar();                            \
      std::stringstream stream(input);                                     \
      stream.unsetf(std::ios::dec);                                        \
      if ((stream.peek() == '-') && std::is_unsigned<type>::value) {       \
        return false;                                                      \
      }                                                                    \
      if (conversion::ConvertStreamTo(stream, rhs)) {                      \
        return true;                                                       \
      }                                                                    \
      if (std::numeric_limits<type>::has_infinity) {                       \
        if (IsInfinity(input)) {                               \
          rhs = std::numeric_limits<type>::infinity();                     \
          return true;                                                     \
        } else if (IsNegativeInfinity(input)) {                \
          rhs = negative_op std::numeric_limits<type>::infinity();         \
          return true;                                                     \
        }                                                                  \
      }                                                                    \
                                                                           \
      if (std::numeric_limits<type>::has_quiet_NaN) {                      \
        if (IsNaN(input)) {                                    \
          rhs = std::numeric_limits<type>::quiet_NaN();                    \
          return true;                                                     \
        }                                                                  \
      }                                                                    \
                                                                           \
      return false;                                                        \
    }                                                                      \
  }

#define YAML_DEFINE_CONVERT_STREAMABLE_SIGNED(type) \
  YAML_DEFINE_CONVERT_STREAMABLE(type, -)

#define YAML_DEFINE_CONVERT_STREAMABLE_UNSIGNED(type) \
  YAML_DEFINE_CONVERT_STREAMABLE(type, +)

YAML_DEFINE_CONVERT_STREAMABLE_SIGNED(int);

YAML_DEFINE_CONVERT_STREAMABLE_SIGNED(short);

YAML_DEFINE_CONVERT_STREAMABLE_SIGNED(long);

YAML_DEFINE_CONVERT_STREAMABLE_SIGNED(long long);

YAML_DEFINE_CONVERT_STREAMABLE_UNSIGNED(unsigned);

YAML_DEFINE_CONVERT_STREAMABLE_UNSIGNED(unsigned short);

YAML_DEFINE_CONVERT_STREAMABLE_UNSIGNED(unsigned long);

YAML_DEFINE_CONVERT_STREAMABLE_UNSIGNED(unsigned long long);

YAML_DEFINE_CONVERT_STREAMABLE_SIGNED(char);

YAML_DEFINE_CONVERT_STREAMABLE_SIGNED(signed char);

YAML_DEFINE_CONVERT_STREAMABLE_UNSIGNED(unsigned char);

YAML_DEFINE_CONVERT_STREAMABLE_SIGNED(float);

YAML_DEFINE_CONVERT_STREAMABLE_SIGNED(double);

YAML_DEFINE_CONVERT_STREAMABLE_SIGNED(long double);

#undef YAML_DEFINE_CONVERT_STREAMABLE_SIGNED
#undef YAML_DEFINE_CONVERT_STREAMABLE_UNSIGNED
#undef YAML_DEFINE_CONVERT_STREAMABLE

// bool
template<>
struct convert<bool> {
    static Node encode(bool rhs) { return rhs ? Node("true") : Node("false"); }

    static bool decode(const Node &node, bool &rhs);
};


// std::pair
template<typename T, typename U>
struct convert<std::pair<T, U>> {
    static Node encode(const std::pair<T, U> &rhs) {
        Node node(NodeType::Sequence);
        node.push_back(rhs.first);
        node.push_back(rhs.second);
        return node;
    }

    static bool decode(const Node &node, std::pair<T, U> &rhs) {
        if (!node.IsSequence())
            return false;
        if (node.size() != 2)
            return false;

        rhs.first = node[0].as<T>();
        rhs.second = node[1].as<U>();

        return true;
    }
};


#endif  // NODE_CONVERT_H_62B23520_7C8E_11DE_8A39_0800200C9A66
