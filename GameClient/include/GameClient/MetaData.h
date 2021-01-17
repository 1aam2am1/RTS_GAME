//
// Created by Michal_Marszalek on 01.11.2020.
//

#ifndef RTS_GAME_METADATA_H
#define RTS_GAME_METADATA_H

#include <GameClient/Unity/Core/Object.h>
#include <GameClient/Unity/Core/Guid.h>
#include <GameApi/function_traits.h>
#include <typeinfo>
#include <unordered_map>
#include <map>
#include <typeindex>
#include <cstdint>
#include <string_view>
#include <GameClient/TPtr.h>
#include <GameApi/SetterGetter.h>
#include <SFML/Graphics/Color.hpp>
#include <GameApi/IsInstance.h>
#include <variant>
#include <SFML/Graphics/Rect.hpp>

class MetaData {
    struct Reflect;
    struct ReflectFull;
public:
    /// Register class meta data with fields
    /// \tparam T Type of class
    /// \param str Name of class
    /// \return
    template<typename T>
    static auto register_class(std::string_view str);

    /// Register class meta data with fields
    /// \tparam T Type of class
    /// \param str Name of class
    /// \param constructor Function that will construct new object
    /// \return
    template<typename T>
    static auto register_class(std::string_view str, std::function<TPtr<T>()> constructor);

    /// Get Reflections of object
    static ReflectFull getReflection(Object *);

    /// Get Reflections of object
    static ReflectFull getReflection(const Object *);

    /// Get Reflection based of class name
    static Reflect getReflection(std::string_view str);

    /// Get Reflection based of class type
    static Reflect getReflection(std::type_index type);

    using flags_type = uintmax_t;
    //std::unordered_map<std::type_index, uintmax_t>

private:
    template<typename T>
    struct Register;

#define FUNC_META_PAIR(TYPE) std::pair<std::function<void(TYPE)>, std::function<TYPE(void)>>

    using TU = std::variant<
            FUNC_META_PAIR(int64_t),
            FUNC_META_PAIR(double),
            FUNC_META_PAIR(std::string),
            FUNC_META_PAIR(bool),
            FUNC_META_PAIR(sf::Color),
            FUNC_META_PAIR(TPtr<Object>),
            FUNC_META_PAIR(nlohmann::json),
            FUNC_META_PAIR(sf::IntRect),
            FUNC_META_PAIR(sf::FloatRect),
            FUNC_META_PAIR(sf::Vector3f),
            FUNC_META_PAIR(sf::Vector3i),
            FUNC_META_PAIR(sf::Vector2f),
            FUNC_META_PAIR(sf::Vector2i),
            FUNC_META_PAIR(std::vector<int64_t>),
            FUNC_META_PAIR(std::vector<std::string>),
            FUNC_META_PAIR(std::vector<TPtr<Object>>)>;

#undef FUNC_META_PAIR

    struct Data {
        std::string_view name{};

        std::function<TPtr<Object>()> create{};
        std::function<bool(Object *, Object *)> copy{};
        std::function<bool(const Object *)> check{};

        std::vector<std::pair<std::string_view, std::function<TU(Object *)>>> members{};
        std::vector<std::pair<std::string_view, std::function<TU(const Object *)>>> c_members{};
        flags_type flags{};
        std::vector<std::pair<std::string_view, const std::type_index>> TPtr_type{};
    };

    static std::map<std::type_index, Data> reflection; ///< type->Data
    static std::map<std::string_view, const std::type_index> name_type; ///< name->type
};

class Importers {
public:
    template<typename T>
    static void register_importer(std::string_view extension);

    template<typename T>
    static void register_importer(int64_t priority);

    static std::pair<const std::type_index, int64_t> get_importer(std::string_view);

    static bool exists_importer(std::string_view);

private:
    static std::map<std::string_view, const std::type_index> ext_importer;
    static std::map<std::type_index, int64_t> ext_priority;
};

class Enums {
    struct Reflect;
public:
    template<typename T>
    static auto register_enum(std::string_view str);

    static Reflect getReflection(std::type_index type);

private:
    template<typename T>
    struct Register;

    struct Data {
        std::vector<std::pair<uint64_t, std::string_view>> members;
    };
    static std::map<std::type_index, Data> reflection;
};

#include "MetaData.inl"

#endif //RTS_GAME_METADATA_H
