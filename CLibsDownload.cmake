cmake_minimum_required(VERSION 3.12)
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)
set(LOCAL_REPO ON CACHE BOOL "Use local repository to copy")

if (${LOCAL_REPO})
    if (WIN32)
        set(SFML_REPO C:/Users/Michal_Marszalek/CLionProjects/SFML)
        set(box2d_REPO C:/Users/Michal_Marszalek/CLionProjects/box2d)
    elseif (UNIX)
        set(SFML_REPO /mnt/c/Users/Michal_Marszalek/CLionProjects/SFML)
        set(box2d_REPO /mnt/c/Users/Michal_Marszalek/CLionProjects/box2d)
    endif ()
else ()
    set(SFML_REPO https://github.com/SFML/SFML)
    set(box2d_REPO https://github.com/erincatto/box2d)
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

### Box2d Dependency ###
FetchContent_Declare(box2d
        GIT_REPOSITORY ${box2d_REPO}
        GIT_SHALLOW TRUE)

FetchContent_GetProperties(box2d)
if (NOT box2d_POPULATED)
    FetchContent_Populate(box2d)
    add_subdirectory(${box2d_SOURCE_DIR} ${box2d_BINARY_DIR})
endif ()


### Protobuf Dependency ###
find_package(Protobuf REQUIRED)
find_package(OpenGL REQUIRED)
