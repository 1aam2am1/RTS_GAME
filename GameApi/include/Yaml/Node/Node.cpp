//
// Created by Michal_Marszalek on 08.10.2020.
//

#include "Node.h"


Node::Node() {
    data = std::make_shared<internal_data>();
}

Node::~Node() {
    //TODO: Clean up
}

Node::Node(const Node &n) {
    data = n.data;
}

NodeType Node::Type() const {
    return data->type;
}

std::string Node::Scalar() const {
    return data->scalar;
}

Node &Node::operator=(const Node &rhs) {
    //TODO: Clean up
    data = rhs.data;
    return *this;
}

void Node::clear() {
    data->scalar.clear();
    data->map.clear();
    data->type = NodeType::Null;
}

void Node::reset(const Node &rhs) {
    data = rhs.data;
    //TODO: Clean up
    //TODO: New shared
}

std::size_t Node::size() const {
    switch (data->type) {
        case NodeType::Null:
        case NodeType::Scalar:
            return 0;
        case NodeType::Sequence:
        case NodeType::Map:
            return data->map.size();
    }
    return 0;
}

Node::const_iterator Node::begin() const {
    return data->map.begin();
}

Node::iterator Node::begin() {
    return data->map.begin();
}

Node::const_iterator Node::end() const {
    return data->map.end();
}

Node::iterator Node::end() {
    return data->map.end();
}

void Node::push_back(const Node &rhs) {
    switch (data->type) {
        case NodeType::Null:
        case NodeType::Sequence:
            data->type = NodeType::Sequence;
            data->map.emplace_back(data->map.size(), rhs);
            return;
        case NodeType::Scalar:
        case NodeType::Map:
            throw std::runtime_error("Wrong push_back");
    }
}

Node &Node::operator[](const Node &key) {
    switch (data->type) {
        case NodeType::Scalar:
            throw std::runtime_error("Wrong operator[]");
        case NodeType::Null:
        case NodeType::Sequence:
        case NodeType::Map:
            data->type = NodeType::Map;

            auto it = std::find_if(data->map.begin(), data->map.end(), [&key](const auto &n) {
                return n.first == key;
            });

            if (it != data->map.end()) {
                return it->second;
            }

            data->map.emplace_back(key, Node{});
            return data->map.back().second;
    }

    throw std::runtime_error("It should not go here");
}

const Node &Node::operator[](const Node &key) const {
    switch (data->type) {
        case NodeType::Scalar:
            throw std::runtime_error("Wrong operator[]");
        case NodeType::Null:
        case NodeType::Sequence:
        case NodeType::Map:
            auto it = std::find_if(data->map.begin(), data->map.end(), [&key](const auto &n) {
                return n.first == key;
            });

            if (it != data->map.end()) {
                return it->second;
            }

            throw std::runtime_error("Wrong operator[] const can't create object");
    }

    throw std::runtime_error("It should not go here");
}

bool Node::remove(const Node &key) {
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

EmitterStyle &Node::style() {
    return data->style;
}

bool Node::operator==(const Node &n) const {
    /*if(data->type == NodeType::Scalar && n.data->type == NodeType::Scalar){
        return data->scalar == n.data->scalar;
    }*/
    return data == n.data;
}
