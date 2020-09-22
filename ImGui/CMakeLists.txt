set(SRC
        imconfig.h
        imgui.cpp
        imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        imgui_stdlib.cpp
        imgui_stdlib.h
        imgui-SFML.cpp
        imgui-SFML.h
        )

add_library(ImGui SHARED ${SRC})


set_target_properties(ImGui PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)


target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ImGui sfml-graphics
        sfml-system
        sfml-window
        ${OPENGL_LIBRARIES})

add_flags(ImGui)


if (WIN32)
    set(GameAPI_DLL ${GameAPI_DLL}
            "${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:ImGui>"
            PARENT_SCOPE)
endif ()
