cmake_minimum_required(VERSION 3.12)
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET ON)
set(LOCAL_REPO ON CACHE BOOL "Use local repository to copy")

if (${LOCAL_REPO})
    if (WIN32)
        set(SFML_REPO C:/Users/Michal_Marszalek/CLionProjects/SFML)
        set(box2d_REPO C:/Users/Michal_Marszalek/CLionProjects/box2d)
        set(JSON_REPO C:/Users/Michal_Marszalek/CLionProjects/json)
    elseif (UNIX)
        set(SFML_REPO /mnt/c/Users/Michal_Marszalek/CLionProjects/SFML)
        set(box2d_REPO /mnt/c/Users/Michal_Marszalek/CLionProjects/box2d)
        set(JSON_REPO /mnt/c/Users/Michal_Marszalek/CLionProjects/json)
    endif ()
else ()
    set(SFML_REPO https://github.com/SFML/SFML)
    set(box2d_REPO https://github.com/erincatto/box2d)
    set(JSON_REPO https://github.com/nlohmann/json)
endif ()

### SFML Dependency ###
set(BUILD_SHARED_LIBS TRUE CACHE BOOL "TRUE to build SFML as shared libraries, FALSE to build it as static libraries" FORCE)
FetchContent_Declare(sfml
        GIT_REPOSITORY ${SFML_REPO}
        GIT_SHALLOW TRUE)

FetchContent_GetProperties(sfml)
if (NOT sfml_POPULATED)
    FetchContent_Populate(sfml)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif ()

#[[
### TGUI Dependency ###
set(TGUI_SHARED_LIBS TRUE CACHE BOOL "TRUE to build TGUI as a shared library, FALSE to build it as a static library" FORCE)
set(TGUI_CXX_STANDARD ${CMAKE_CXX_STANDARD} CACHE STRING "C++ standard version to build TGUI with. Possible values: 14, 17 or 20. Projects using TGUI must use a version equal or higher to this")
FetchContent_Declare(tgui
        GIT_REPOSITORY https://github.com/texus/TGUI
        GIT_TAG origin/0.8
        GIT_SHALLOW TRUE)

FetchContent_GetProperties(tgui)
if (NOT tgui_POPULATED)
    FetchContent_Populate(tgui)
    add_subdirectory(${tgui_SOURCE_DIR} ${tgui_BINARY_DIR})
endif ()
#]]

### JSON Dependency ###
set(JSON_BuildTests FALSE CACHE BOOL "" FORCE)
set(JSON_Install FALSE CACHE BOOL "" FORCE)
set(JSON_MultipleHeaders TRUE CACHE BOOL "" FORCE)
set(JSON_ImplicitConversions FALSE CACHE BOOL "" FORCE)
set(JSON_BUILD_SHARED_LIBS TRUE CACHE BOOL "TRUE to build TGUI as a shared library, FALSE to build it as a static library" FORCE)
FetchContent_Declare(json
        GIT_REPOSITORY ${JSON_REPO}
        GIT_SHALLOW TRUE
        GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if (NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR})
endif ()

### Box2d Dependency ###
set(BOX2D_BUILD_TESTBED FALSE CACHE BOOL "" FORCE)
FetchContent_Declare(box2d
        GIT_REPOSITORY ${box2d_REPO}
        GIT_SHALLOW TRUE)

FetchContent_GetProperties(box2d)
if (NOT box2d_POPULATED)
    FetchContent_Populate(box2d)
    add_subdirectory(${box2d_SOURCE_DIR} ${box2d_BINARY_DIR})
endif ()


### Protobuf Dependency ###
#ind_package(Protobuf REQUIRED)
find_package(OpenGL REQUIRED)
