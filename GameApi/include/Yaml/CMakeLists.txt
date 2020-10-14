set(SRC
        Node/Node.cpp
        Node/Node.h
        Node/Node.inl
        Node/NodeType.h
        Node/EmitterStyle.h
        Node/convert.h
        Node/convert.cpp
        Node/NodeIterator.cpp
        Node/NodeIterator.h
        Emitter.cpp
        Emitter.h
        Document.cpp
        Document.h
        )

add_library(YAML STATIC ${SRC})


set_target_properties(YAML PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

target_include_directories(YAML PUBLIC
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_flags(YAML)

#[[
set_source_files_properties(${PROTO_HDRS} ${PROTO_SRCS} PROPERTIES COMPILE_FLAGS "-Wno-all \
-Wno-extra \
-Wno-pedantic \
-Wno-return-local-addr \
-Wno-attributes \
-Wno-deprecated")
#]]
