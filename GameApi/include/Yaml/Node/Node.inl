
template<typename T>
struct convert;

template<typename T>
Node::Node(const T &t) : Node() {
    *this = t;
}

template<typename T>
void Node::push_back(const T &rhs) {
    push_back(Node{rhs});
}

template<typename T>
T Node::as() const {
    T t;
    if (convert<T>::decode(*this, t))
        return t;

    throw std::runtime_error("Bad conversion");
}

template<typename T, typename Y>
T Node::as(const Y &fallback) const {
    T t;
    if (convert<T>::decode(*this, t))
        return t;

    return fallback;
}

template<typename T>
Node &Node::operator=(const T &rhs) {
    if constexpr (std::is_convertible_v<T, std::string_view>) {
        data->type = NodeType::Scalar;
        data->scalar = rhs;
    } else {
        auto d = convert<T>::encode(rhs);
        *data = *d.data;
    }

    return *this;
}

template<typename Key>
bool Node::remove(const Key &key) {
    switch (data->type) {
        case NodeType::Scalar:
            throw std::runtime_error("Wrong push_back");
        case NodeType::Null:
        case NodeType::Sequence:
        case NodeType::Map: {
            auto it = std::find_if(data->map.begin(), data->map.end(), [&key](const auto &n) {
                return n.first == key;
            });

            if (it != data->map.end()) {
                it = data->map.erase(it);
                if (data->map.empty()) {
                    data->type = NodeType::Null;
                } else {
                    if (it != data->map.end()) {
                        data->type = NodeType::Map;
                    }
                }
                return true;
            }

            return false;
        }
    }

    throw std::runtime_error("It should not go here");
}

template<typename Key>
Node &Node::operator[](const Key &key) {
    switch (data->type) {
        case NodeType::Scalar:
            throw std::runtime_error("Wrong operator[]");
        case NodeType::Null:
        case NodeType::Sequence: {
            //TODO: You know where it is therefore faster
            if constexpr (std::is_integral_v<Key> && !std::is_same_v<Key, bool>) {
                size_t uk = key;
                if (key >= 0 && uk <= data->map.size()) {
                    data->type = NodeType::Sequence;
                    break;
                }
            }
            data->type = NodeType::Map;
        }
        case NodeType::Map:
            break;
    }

    auto it = std::find_if(data->map.begin(), data->map.end(), [&key](const auto &n) {
        return n.first == key;
    });

    if (it != data->map.end()) {
        return it->second;
    }

    data->map.emplace_back(Node{key}, Node{});
    return data->map.back().second;
}

template<typename Key>
const Node &Node::operator[](const Key &key) const {
    switch (data->type) {
        case NodeType::Scalar:
            throw std::runtime_error("Wrong operator[]");
        case NodeType::Null:
        case NodeType::Sequence: {
            //TODO: You know where it is therefore faster
            if constexpr (std::is_integral_v<Key> && !std::is_same_v<Key, bool>) {
                size_t uk = key;
                if (key >= 0 && uk <= data->map.size()) {
                    data->type = NodeType::Sequence;
                    break;
                }
            }
            data->type = NodeType::Map;
        }
        case NodeType::Map:
            break;
    }

    auto it = std::find_if(data->map.begin(), data->map.end(), [&key](const auto &n) {
        return n.first == key;
    });

    if (it != data->map.end()) {
        return it->second;
    }

    throw std::runtime_error("Wrong operator[] const can't create object");
}

template<typename Key, typename Value>
void Node::force_insert(const Key &key, const Value &value) {
    switch (data->type) {
        case NodeType::Scalar:
            throw std::runtime_error("Wrong operator[]");
        case NodeType::Null:
        case NodeType::Sequence:
        case NodeType::Map:
            data->type = NodeType::Map;

            data->map.emplace_back(Node{key}, Node{value});
            return;
    }

    throw std::runtime_error("It should not go here");
}

template<typename Key>
bool Node::equal(const Key &key) const {
    if constexpr (std::is_convertible_v<Key, std::string_view>) {
        return Scalar() == key;
    } else {
        return as<Key>() == key;
    }

}

template<typename Key>
bool Node::operator==(const Key &key) const {
    return equal(key);
}

template<typename Key>
bool Node::operator!=(const Key &key) const {
    return !equal(key);
}
