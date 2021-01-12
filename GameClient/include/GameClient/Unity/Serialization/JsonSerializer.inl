template<typename T>
nlohmann::json JsonSerializer::operator()(T &&t) {
    return t;
}

//Should I specialize TPtr vector like in scene serializer?
template<typename T>
nlohmann::json JsonSerializer::operator()(std::vector<T> vec) {
    nlohmann::json::array_t result;
    result.reserve(vec.size());

    for (auto &it : vec) {
        result.emplace_back(operator()(it));
    }

    return result;
}


template<typename T>
requires std::is_default_constructible_v<T>
void JsonSerializer::operator()(const std::function<void(T)> &f, const nlohmann::json &j) {
    T def;
    try {
        def = j.get<T>();
    } catch (...) {
        def = T{};
    }
    f(def);
}

template<typename T>
void JsonSerializer::operator()(const std::function<void(std::vector<T>)> &f,
                                const nlohmann::json &j) {
    std::shared_ptr<std::vector<T>> value(new std::vector<T>,
                                          [f](auto ptr) {
                                              f(*ptr);
                                              delete ptr;
                                          });

    if (j.is_array()) {
        value->resize(j.size());
        int i = 0;
        for (auto &v : j.items()) {
            std::function<void(T)> func = [value, i](auto n) -> void {
                (*value)[i] = n;
            };
            this->operator()(func, v.value());
            ++i;
        }
    }
}
