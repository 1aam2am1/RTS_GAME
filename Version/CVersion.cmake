cmake_minimum_required(VERSION 3.12)

# Generate version flags
find_package(Git QUIET REQUIRED)
execute_process(COMMAND ${GIT_EXECUTABLE} status --porcelain
        OUTPUT_VARIABLE BUILD_STATE
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=0 --dirty --tags
        OUTPUT_VARIABLE _GIT_DESCRIBE
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
        OUTPUT_VARIABLE _GIT_HASH
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

string(TIMESTAMP _TIME "%Y-%m-%d %H:%M:%S")

if (BUILD_STATE)
    set(DIRTY "-dirty")
else ()
    set(DIRTY "")
endif ()


file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
        "#include \"Version/version.h\"\n"
        "\n"
        "\n"
        "const std::string_view Version::GIT_DIRTY = \"${DIRTY}\";\n"
        "const std::string_view Version::GIT_SHA = \"${_GIT_HASH}\";\n"
        "const std::string_view Version::GIT_TAG = \"${_GIT_DESCRIBE}\";\n"
        "const std::string_view Version::DATE = \"${_TIME}\";\n")
